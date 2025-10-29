#ifdef USE_SHADER_METAL
#include "MetalRaytracer.hpp"

#include <iostream>

#include <Metal/MTLLibrary.hpp>
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLCommandQueue.hpp>
#include <Metal/MTLComputePipeline.hpp>
#include <simd/simd.h>
#include <Metal/MTLBuffer.hpp>

#include "../Renderer.h"
#include "../timing.hpp"
#include "../../shader/metal/shader_types.hpp"


namespace RayTracing {
    MetalRaytracer::MetalRaytracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel)
        : RayTracer(windowSize, bounces, samplesPerPixel) {
        pAutoreleasePool = NS::AutoreleasePool::alloc()->init();
        initialize();
    }

    MetalRaytracer::~MetalRaytracer() {
        pAutoreleasePool->release();
    }

    void MetalRaytracer::initialize() {
        auto windowSize = getWindowSize();
        device = MTL::CreateSystemDefaultDevice();

        defaultLibrary = device->newDefaultLibrary();
        if (!defaultLibrary) {
            std::cerr << "Error creating default library" << std::endl;
            return;
        }

        commandQueue = device->newCommandQueue();
        if (!commandQueue) {
            std::cerr << "Error creating command queue" << std::endl;
            return;
        }

        auto *pixelCoords = new simd::uint2[windowSize.getX() * windowSize.getY()];
        auto *uvs = new simd::float2[windowSize.getX() * windowSize.getY()];
        for (unsigned y = 0; y < windowSize.getY(); ++y) {
            for (unsigned x = 0; x < windowSize.getX(); ++x) {
                pixelCoords[y * windowSize.getX() + x] = simd::uint2{x, y};
                uvs[y * windowSize.getX() + x] = simd::float2{
                    (float) x / (float) windowSize.getX(), (float) y / (float) windowSize.getY()
                };
            }
        }

        auto *screenSize = new simd::uint2{windowSize.getX(), windowSize.getY()};
        bufferScreenSize = device->newBuffer(sizeof(simd::uint2), MTL::ResourceStorageModeShared);
        memcpy(bufferScreenSize->contents(), screenSize, sizeof(simd::uint2));

        bufferForward = device->newBuffer(sizeof(simd::float3), MTL::ResourceStorageModeShared);
        auto *forward = new simd::float3{Vec3::forward().getX(), Vec3::forward().getY(), Vec3::forward().getZ()};
        memcpy(bufferForward->contents(), forward, sizeof(simd::float3));

        bufferResult = device->newBuffer(
            sizeof(simd::float4) * windowSize.getX() * windowSize.getY() * getSamplesPerPixel(),
            MTL::ResourceStorageModeShared);

        bufferRays = device->newBuffer(sizeof(Metal_Ray) * windowSize.getX() * windowSize.getY() * getSamplesPerPixel(),
                                       MTL::ResourceStorageModeShared);

        // data to be filled on encode
        bufferRayTraceSettings = device->newBuffer(sizeof(Metal_RayTraceSettings), MTL::ResourceStorageModeShared);
    }

    MetalRaytracer::KernelFunctionVariables MetalRaytracer::loadFunction(const std::string &name) {
        NS::Error *error = nullptr;
        MTL::Function *kernelFunction = defaultLibrary->newFunction(
            NS::String::string(name.c_str(), NS::UTF8StringEncoding));
        if (!kernelFunction) {
            std::cerr << "Failed to find the " << name << " function" << std::endl;
            return {};
        }
        auto *kernelFunctionPSO = device->newComputePipelineState(kernelFunction, &error);
        if (!kernelFunctionPSO) {
            std::cerr << "Failed to created pipeline state object, error" << error->localizedDescription() << std::endl;
            kernelFunction->release();
            return {};
        }
        return KernelFunctionVariables{kernelFunction, kernelFunctionPSO};
    }

    void MetalRaytracer::sendComputeCommand(MetalEncodingData data,
                                            void (MetalRaytracer::*function)(
                                                MetalEncodingData, MTL::ComputeCommandEncoder *)) {
        MTL::CommandBuffer *commandBuffer = commandQueue->commandBuffer();
        assert(commandBuffer != nullptr);

        MTL::ComputeCommandEncoder *computeEncoder = commandBuffer->computeCommandEncoder();
        assert(computeEncoder != nullptr);

        (this->*function)(data, computeEncoder);

        TIMING_START(executeCompute)
        computeEncoder->endEncoding();
        commandBuffer->commit();
        commandBuffer->waitUntilCompleted();
        TIMING_END(executeCompute)
        TIMING_LOG(executeCompute, identifier(), "Compute command execution")
    }

    void MetalRaytracer::encodeUVTestData(MetalEncodingData data,
                                          MTL::ComputeCommandEncoder *computeEncoder) {
        auto [function, functionPSO] = data.variables;
        computeEncoder->setComputePipelineState(functionPSO);
        computeEncoder->setBuffer(bufferResult, 0, 0);
        computeEncoder->setBuffer(bufferScreenSize, 0, 1);

        MTL::Size gridSize = MTL::Size(getWindowSize().getX(), getWindowSize().getY(), 1);

        NS::UInteger maxThreadGroupSize = functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size tGroupSize = MTL::Size::Make(maxThreadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, tGroupSize);
    }

    Image *MetalRaytracer::uvTest() {
        auto variables = loadFunction("uvTest");
        sendComputeCommand({variables, nullptr}, &MetalRaytracer::encodeUVTestData);

        return outputBufferToImage(1);
    }

    void MetalRaytracer::encodeRayTestData(MetalEncodingData data,
                                           MTL::ComputeCommandEncoder *computeEncoder) {
        auto [function, functionPSO] = data.variables;
        const auto rays = calculateStartingRays(data.scene->camera);
        const auto metalRays = raysToMetal(rays);
        const auto rayArray = metalRays.data();
        memcpy(bufferRays->contents(), rayArray, metalRays.size() * sizeof(Metal_Ray));

        computeEncoder->setComputePipelineState(functionPSO);
        computeEncoder->setBuffer(bufferRays, 0, 0);
        computeEncoder->setBuffer(bufferForward, 0, 1);
        computeEncoder->setBuffer(bufferResult, 0, 2);

        MTL::Size gridSize = MTL::Size::Make(getWindowSize().getX(), getWindowSize().getY(), getSamplesPerPixel());

        NS::UInteger maxThreadGroupSize = functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size tGroupSize = MTL::Size::Make(maxThreadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, tGroupSize);
    }

    Image *MetalRaytracer::rayTest(Camera *camera) {
        const auto variables = loadFunction("rayTest");
        assert(variables.functionPSO != nullptr);

        const auto scene = new Scene{camera};

        const auto data = MetalEncodingData{
            .variables = variables,
            .scene = scene
        };
        sendComputeCommand(data, &MetalRaytracer::encodeRayTestData);
        delete scene;

        return outputBufferToImage(getSamplesPerPixel());
    }

    Image *MetalRaytracer::outputBufferToImage(unsigned samples) {
        auto windowSize = getWindowSize();
        auto *image = new Image(windowSize.getX(), windowSize.getY());
        for (unsigned y = 0; y < windowSize.getY(); y++) {
            for (unsigned x = 0; x < windowSize.getX(); x++) {
                std::vector<RGBf> colors;
                int startIndex = (y * getWindowSize().getX() + x) * samples;
                for (unsigned s = 0; s < samples; s++) {
                    simd::float4 color = ((simd::float4 *) bufferResult->contents())[startIndex + s];
                    colors.push_back(RGBf::fromFloat4(color));
                }
                image->setPixel(x, y, RGBf::blend(colors));
            }
        }
        return image;
    }

    std::vector<Metal_Ray> MetalRaytracer::raysToMetal(const std::vector<Ray> &rays) {
        std::vector<Metal_Ray> result;
        for (auto &ray: rays) {
            result.push_back(Metal_Ray(
                simd::float3{ray.origin.getX(), ray.origin.getY(), ray.origin.getZ()},
                simd::float3{ray.direction.getX(), ray.direction.getY(), ray.direction.getZ()},
                {},
                0
            ));
        }
        return result;
    }

    MetalRaytracer::Metal_MeshTransformationReturn MetalRaytracer::meshObjectsToMetal(
        const std::vector<MeshedRayTraceableObject *> &objects) {
        std::vector<Metal_MeshRayTraceableObject> meshObjects;
        std::vector<simd::float3> vertices;
        std::vector<int> indices;
        std::vector<simd::float3> normals;

        for (auto &object: objects) {
            meshObjects.push_back(Metal_MeshRayTraceableObject{
                .boundingBox = object->boundingBox.toMetal(),
                .transform = object->transform.getTransformMatrix().toMetal(),
                .inverseTransform = object->transform.getInverseTransformMatrix().toMetal(),
                .rotation = object->transform.getRotationMatrix().toMetal(),
                .inverseRotate = object->transform.getInverseRotationMatrix().toMetal(),
                .inverseScale = object->transform.getInverseScaleMatrix().toMetal(),
                .color = object->color.toMetal(),
                .indicesOffset = (unsigned) indices.size(),
                .triangleCount = (unsigned) object->mesh->numTriangles,
                .vertexOffset = (unsigned) vertices.size(),
                .normalsOffset = (unsigned) normals.size(),
            });
            for (auto &vertex: object->mesh->vertices) {
                vertices.push_back(vertex.toMetal());
            }
            for (auto &normal: object->mesh->normals) {
                normals.push_back(normal.toMetal());
            }
            indices.insert(indices.end(), object->mesh->indices.begin(), object->mesh->indices.end());
        }
        return {
            .meshObjects = meshObjects,
            .vertices = vertices,
            .indices = indices,
        };
    }

    std::vector<Metal_SphereRayTraceableObject> MetalRaytracer::sphereObjectsToMetal(
        const std::vector<SphereRayTraceableObject *> &objects) {
        std::vector<Metal_SphereRayTraceableObject> result;
        for (auto &object: objects) {
            result.push_back(Metal_SphereRayTraceableObject{
                .boundingBox = object->boundingBox.toMetal(),
                .center = object->transform.getTranslation().toMetal(),
                .radius = object->radius,
                .color = object->color.toMetal(),
            });
        }
        return result;
    }

    std::vector<Metal_Light> MetalRaytracer::lightsToMetal(const std::vector<LightSource *> &lights) {
        std::vector<Metal_Light> result;
        for (auto &light: lights) {
            result.push_back(Metal_Light{
                .boundingBox = light->boundingBox.toMetal(),
                .center = light->transform.getTranslation().toMetal(),
                .intensity = 1,
                .color = light->emittingColor.toMetal(),
                .radius = light->radius
            });
        }
        return result;
    }

    void prepBuffer(MTL::Buffer **buffer, MTL::Device *device, size_t size) {
        if (*buffer != nullptr)
            (*buffer)->release();
        *buffer = device->newBuffer(size, MTL::ResourceStorageModeShared);
    }

    void MetalRaytracer::encodeRaytracingData(MetalEncodingData data,
                                              MTL::ComputeCommandEncoder *computeEncoder) {
        auto [function, functionPSO] = data.variables;
        auto scene = *(data.scene);

        TIMING_START(prepBuffers)

        // prep data for buffers
        auto rays = calculateStartingRays(scene.camera);
        auto metalRays = raysToMetal(rays);
        auto meshObjects = meshObjectsToMetal(scene.objects);
        auto sphereObjects = sphereObjectsToMetal(scene.spheres);
        auto lights = lightsToMetal(scene.lights);
        auto *settings = new Metal_RayTraceSettings{
            .screenSize = {getWindowSize().getX(), getWindowSize().getY()},
            .bounces = getBounces(),
            .samplesPerPixel = getSamplesPerPixel(),
            .meshObjectCount = (unsigned) meshObjects.meshObjects.size(),
            .sphereObjectCount = (unsigned) sphereObjects.size(),
            .lightsCount = (unsigned) lights.size(),
            .maxColorsPerRay = 6
        };


        // prep buffers
        prepBuffer(&bufferMeshObjects, device, sizeof(Metal_MeshRayTraceableObject) * meshObjects.meshObjects.size());
        prepBuffer(&bufferMeshVertices, device, sizeof(simd::float3) * meshObjects.vertices.size());
        prepBuffer(&bufferMeshIndices, device, sizeof(int) * meshObjects.indices.size());
        prepBuffer(&bufferNormals, device, sizeof(simd::float3) * meshObjects.normals.size());
        prepBuffer(&bufferSphereObjects, device, sizeof(Metal_SphereRayTraceableObject) * sphereObjects.size());
        prepBuffer(&bufferLights, device, sizeof(Metal_Light) * lights.size());


        // copy data to buffers
        memcpy(bufferRayTraceSettings->contents(), settings, sizeof(Metal_RayTraceSettings));
        memcpy(bufferRays->contents(), metalRays.data(), metalRays.size() * sizeof(Metal_Ray));
        memcpy(bufferMeshObjects->contents(), meshObjects.meshObjects.data(),
               meshObjects.meshObjects.size() * sizeof(Metal_MeshRayTraceableObject));
        memcpy(bufferMeshVertices->contents(), meshObjects.vertices.data(),
               meshObjects.vertices.size() * sizeof(simd::float3));
        memcpy(bufferMeshIndices->contents(), meshObjects.indices.data(),
               meshObjects.indices.size() * sizeof(int));
        memcpy(bufferNormals->contents(), meshObjects.normals.data(),
               meshObjects.normals.size() * sizeof(simd::float3));
        memcpy(bufferSphereObjects->contents(), sphereObjects.data(),
               sphereObjects.size() * sizeof(Metal_SphereRayTraceableObject));
        memcpy(bufferLights->contents(), lights.data(), lights.size() * sizeof(Metal_Light));

        // set function arguments
        computeEncoder->setComputePipelineState(functionPSO);

        computeEncoder->setBuffer(bufferRayTraceSettings, 0, 0);
        computeEncoder->setBuffer(bufferRays, 0, 1);
        computeEncoder->setBuffer(bufferMeshObjects, 0, 2);
        computeEncoder->setBuffer(bufferMeshVertices, 0, 3);
        computeEncoder->setBuffer(bufferMeshIndices, 0, 4);
        computeEncoder->setBuffer(bufferNormals, 0, 5);
        computeEncoder->setBuffer(bufferSphereObjects, 0, 6);
        computeEncoder->setBuffer(bufferLights, 0, 7);
        computeEncoder->setBuffer(bufferResult, 0, 8);

        TIMING_END(prepBuffers)
        TIMING_LOG(prepBuffers, identifier(), "Preparing and encoding data into buffers for raytracing")

        MTL::Size gridSize = MTL::Size::Make(getWindowSize().getX(), getWindowSize().getY(), getSamplesPerPixel());

        NS::UInteger maxThreadGroupSize = functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size tGroupSize = MTL::Size::Make(maxThreadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, tGroupSize);
    }

    Image *MetalRaytracer::raytrace(Scene scene) {
        TIMING_START(prepping)
        float maxDepth = 0;
        unsigned long triangleCount = 0;
        for (auto &object: scene.objects) {
            object->updateBoundingBox();
            object->transform.update();
            object->updateNestedBoundingBox(300);
            maxDepth = std::max(maxDepth, (float) object->nestedBoundingBox.depth());
            triangleCount += object->mesh->numTriangles;
        }
        TIMING_END(prepping)
        TIMING_LOG(prepping, identifier(), "prepping scene for raytracing")
        std::cout << "[" << identifier() << "] Starting raytrace with "
                << getWindowSize().getX() * getWindowSize().getY() * getSamplesPerPixel() << " rays, "
                << scene.objects.size() << " mesh objects (" << triangleCount << " triangles), "
                << scene.spheres.size() << " spheres and "
                << scene.lights.size() << " light sources"
                << std::endl;
        std::cout << "[" << identifier() << "]" << " Maximum nested bounding box depth: " << maxDepth << std::endl;

        const auto variables = loadFunction("raytrace");

        sendComputeCommand({variables, &scene}, &MetalRaytracer::encodeRaytracingData);
        return outputBufferToImage(getSamplesPerPixel());
    }
}
#endif