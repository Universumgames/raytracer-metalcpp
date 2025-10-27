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
        bufferPixel = device->newBuffer(sizeof(simd::uint2) * windowSize.getX() * windowSize.getY(),
                                        MTL::ResourceStorageModeShared);
        memcpy(bufferPixel->contents(), pixelCoords, sizeof(simd::uint2) * windowSize.getX() * windowSize.getY());

        auto *screenSize = new simd::uint2{windowSize.getX(), windowSize.getY()};
        bufferScreenSize = device->newBuffer(sizeof(simd::uint2), MTL::ResourceStorageModeShared);
        memcpy(bufferScreenSize->contents(), screenSize, sizeof(simd::uint2));

        bufferUV = device->newBuffer(sizeof(simd::float2) * windowSize.getX() * windowSize.getY(),
                                     MTL::ResourceStorageModeShared);
        memcpy(bufferUV->contents(), uvs, sizeof(simd::float2) * windowSize.getX() * windowSize.getY());

        bufferResult = device->newBuffer(sizeof(simd::float4) * windowSize.getX() * windowSize.getY(),
                                         MTL::ResourceStorageModeShared);

        bufferRays = device->newBuffer(sizeof(Metal_Ray) * windowSize.getX() * windowSize.getY(),
                                       MTL::ResourceStorageModeShared);
        bufferForward = device->newBuffer(sizeof(simd::float3), MTL::ResourceStorageModeShared);
        auto *forward = new simd::float3{Vec3::forward().getX(), Vec3::forward().getY(), Vec3::forward().getZ()};
        memcpy(bufferForward->contents(), forward, sizeof(simd::float3));

        bufferRayTraceSettings = device->newBuffer(sizeof(Metal_RayTraceSettings), MTL::ResourceStorageModeShared);
        // data to be filled on encode
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

        computeEncoder->endEncoding();
        commandBuffer->commit();
        commandBuffer->waitUntilCompleted();
    }

    void MetalRaytracer::encodeUVTestData(MetalEncodingData data,
                                          MTL::ComputeCommandEncoder *computeEncoder) {
        auto variables = data.variables;
        computeEncoder->setComputePipelineState(variables.functionPSO);
        computeEncoder->setBuffer(bufferResult, 0, 0);
        computeEncoder->setBuffer(bufferScreenSize, 0, 1);

        MTL::Size gridSize = MTL::Size(windowSize.getX(), windowSize.getY(), 1);

        NS::UInteger threadGroupSize = variables.functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size threadgroupSize(threadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    }

    Image *MetalRaytracer::uvTest() {
        auto variables = loadFunction("uvTest");
        sendComputeCommand({variables, nullptr}, &MetalRaytracer::encodeUVTestData);

        return outputBufferToImage();
    }

    void MetalRaytracer::encodeRayTestData(MetalEncodingData data,
                                           MTL::ComputeCommandEncoder *computeEncoder) {
        auto variables = data.variables;
        auto rays = calculateStartingRays(data.scene->camera);
        auto metalRays = raysToMetal(rays);
        auto rayArray = metalRays.data();
        memcpy(bufferRays->contents(), rayArray, windowSize.getX() * windowSize.getY() * sizeof(Metal_Ray));

        computeEncoder->setComputePipelineState(variables.functionPSO);
        computeEncoder->setBuffer(bufferRays, 0, 0);
        computeEncoder->setBuffer(bufferForward, 0, 1);
        computeEncoder->setBuffer(bufferResult, 0, 2);

        MTL::Size gridSize = MTL::Size::Make(windowSize.getX(), windowSize.getY(), 1);

        NS::UInteger threadGroupSize = variables.functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    }

    Image *MetalRaytracer::rayTest(Camera *camera) {
        auto variables = loadFunction("rayTest");
        assert(variables.functionPSO != nullptr);

        auto scene = new Scene{camera};

        auto data = MetalEncodingData{
            .variables = variables,
            .scene = scene
        };
        sendComputeCommand(data, &MetalRaytracer::encodeRayTestData);
        delete scene;

        return outputBufferToImage();
    }

    Image *MetalRaytracer::outputBufferToImage() {
        auto *image = new Image(windowSize.getX(), windowSize.getY());
        for (unsigned y = 0; y < windowSize.getY(); y++) {
            for (unsigned x = 0; x < windowSize.getX(); x++) {
                simd::float4 color = ((simd::float4 *) bufferResult->contents())[y * windowSize.getX() + x];
                image->setPixel(x, y, RGBf::fromFloat4(color).toRGBA8());
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
                {}
            ));
        }
        return result;
    }

    MetalRaytracer::Metal_MeshTransformationReturn MetalRaytracer::meshObjectsToMetal(
        const std::vector<MeshedRayTraceableObject *> &objects) {
        std::vector<Metal_MeshRayTraceableObject> meshObjects;
        std::vector<simd::float3> vertices;
        std::vector<int> indices;

        for (auto &object: objects) {
            meshObjects.push_back(Metal_MeshRayTraceableObject{
                .transform = object->transform.getTransformMatrix().toMetal(),
                .color = object->color.toMetal(),
                .vertIndicesOffset = (unsigned) indices.size(),
                .vertIndicesCount = (unsigned) object->mesh->indices.size(),
                .vertPositionsOffset = (unsigned) vertices.size(),
            });
            for (auto &vertex: object->mesh->vertices) {
                vertices.push_back(vertex.toMetal());
            }
            for (auto &index: object->mesh->indices) {
                indices.push_back(index);
            }
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
                .transform = object->transform.getTransformMatrix().toMetal(),
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
                .transform = light->transform.getTransformMatrix().toMetal(),
                .intensity = 1,
                .color = light->emittingColor.toMetal(),
                .radius = light->radius
            });
        }
        return result;
    }

    void MetalRaytracer::encodeRaytracingData(MetalEncodingData data,
                                              MTL::ComputeCommandEncoder *computeEncoder) {
        auto variables = data.variables;
        auto scene = *(data.scene);
        computeEncoder->setComputePipelineState(variables.functionPSO);

        auto rays = calculateStartingRays(scene.camera);
        auto meshObjects = meshObjectsToMetal(scene.objects);
        auto sphereObjects = sphereObjectsToMetal(scene.spheres);
        auto lights = lightsToMetal(scene.lights);

        auto *settings = new Metal_RayTraceSettings{
            .bounces = this->bounces,
            .samplesPerPixel = this->samplesPerPixel,
            .meshObjectCount = 0,
            .sphereObjectCount = 0,
            .lightsCount = 0
        };
        memcpy(bufferRayTraceSettings->contents(), settings, sizeof(Metal_RayTraceSettings));
        TODO(); // TODO Finish encoding scene data
    }

    Image *MetalRaytracer::raytrace(Scene scene) {
        auto *image = new Image(windowSize.getX(), windowSize.getY());
        auto variables = loadFunction("raytrace");

        sendComputeCommand({variables, &scene}, &MetalRaytracer::encodeRaytracingData);
        outputBufferToImage();
        return image;
    }
}
#endif