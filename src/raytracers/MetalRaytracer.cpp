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
    MetalRaytracer::MetalRaytracer(Vec2u windowSize, unsigned bounces, unsigned samplesPerPixel)
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
    }

    MetalRaytracer::KernelFunctionVariables *MetalRaytracer::loadFunction(const std::string &name) {
        NS::Error *error = nullptr;
        MTL::Function *kernelFunction = defaultLibrary->newFunction(
            NS::String::string(name.c_str(), NS::UTF8StringEncoding));
        if (!kernelFunction) {
            std::cerr << "Failed to find the " << name << " function" << std::endl;
            return nullptr;
        }
        auto *kernelFunctionPSO = device->newComputePipelineState(kernelFunction, &error);
        if (!kernelFunctionPSO) {
            std::cerr << "Failed to created pipeline state object, error" << error->localizedDescription() << std::endl;
            kernelFunction->release();
            return nullptr;
        }
        return new KernelFunctionVariables{kernelFunction, kernelFunctionPSO};
    }

    void MetalRaytracer::sendComputeCommand(KernelFunctionVariables *variables,
                                            void (MetalRaytracer::*function)(
                                                KernelFunctionVariables *, MTL::ComputeCommandEncoder *)) {
        MTL::CommandBuffer *commandBuffer = commandQueue->commandBuffer();
        assert(commandBuffer != nullptr);

        MTL::ComputeCommandEncoder *computeEncoder = commandBuffer->computeCommandEncoder();
        assert(computeEncoder != nullptr);

        (this->*function)(variables, computeEncoder);

        computeEncoder->endEncoding();
        commandBuffer->commit();
        commandBuffer->waitUntilCompleted();
    }

    void MetalRaytracer::encodeUVTestData(KernelFunctionVariables *variables,
                                          MTL::ComputeCommandEncoder *computeEncoder) {
        computeEncoder->setComputePipelineState(variables->functionPSO);
        computeEncoder->setBuffer(bufferResult, 0, 0);
        computeEncoder->setBuffer(bufferScreenSize, 0, 1);

        MTL::Size gridSize = MTL::Size(windowSize.getX(), windowSize.getY(), 1);

        NS::UInteger threadGroupSize = variables->functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size threadgroupSize(threadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    }

    Image *MetalRaytracer::uvTest() {
        auto *variables = loadFunction("uvTest");
        sendComputeCommand(variables, &MetalRaytracer::encodeUVTestData);

        return outputBufferToImage();
    }

    void MetalRaytracer::encodeRaytracingData(KernelFunctionVariables *variables,
                                              MTL::ComputeCommandEncoder *computeEncoder) {
        computeEncoder->setComputePipelineState(variables->functionPSO);
        // TODO encode scene data
    }

    Image *MetalRaytracer::raytrace(Scene scene) {
        auto *image = new Image(windowSize.getX(), windowSize.getY());
        auto variables = loadFunction("raytrace");
        auto rays = calculateStartingRays(scene.camera);


        sendComputeCommand(variables, &MetalRaytracer::encodeRaytracingData);
        // TODO prepare date for gpu
        // TODO run on gpu
        // TODO extract image from buffer
        outputBufferToImage();
        return image;
    }

    void MetalRaytracer::encodeRayTestData(KernelFunctionVariables *variables,
                                           MTL::ComputeCommandEncoder *computeEncoder) {
        auto rayArray = variables->metalRays->data();
        memcpy(bufferRays->contents(), rayArray, windowSize.getX() * windowSize.getY() * sizeof(Metal_Ray));

        computeEncoder->setComputePipelineState(variables->functionPSO);
        computeEncoder->setBuffer(bufferRays, 0, 0);
        computeEncoder->setBuffer(bufferForward, 0, 1);
        computeEncoder->setBuffer(bufferResult, 0, 2);

        MTL::Size gridSize = MTL::Size::Make(windowSize.getX(), windowSize.getY(), 1);

        NS::UInteger threadGroupSize = variables->functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    }


    Image *MetalRaytracer::rayTest(Camera *camera) {
        auto variables = loadFunction("rayTest");
        assert(variables != nullptr);
        auto rays = calculateStartingRays(camera);

        auto metalRays = raysToMetal(rays);
        variables->metalRays = &metalRays;
        sendComputeCommand(variables, &MetalRaytracer::encodeRayTestData);

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
}
#endif
