#ifdef USE_SHADER_METAL
#include "MetalRaytracer.hpp"

#include <iostream>

#include <Metal/MTLLibrary.hpp>
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLCommandQueue.hpp>
#include <Metal/MTLComputePipeline.hpp>
#include <simd/simd.h>

#include "Metal/MTLBuffer.hpp"


namespace RayTracing {
    MetalRaytracer::MetalRaytracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel)
        : RayTracer(width, height, bounces, samplesPerPixel) {
        pAutoreleasePool = NS::AutoreleasePool::alloc()->init();
        initialize();
    }

    MetalRaytracer::~MetalRaytracer() {
        pAutoreleasePool->release();
    }

    void MetalRaytracer::initialize() {
        NS::Error *error = nullptr;
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

        auto *pixelCoords = new simd::uint2[width * height];
        auto *uvs = new simd::float2[width * height];
        for (unsigned y = 0; y < height; ++y) {
            for (unsigned x = 0; x < width; ++x) {
                pixelCoords[y * width + x] = simd::uint2{x, y};
                uvs[y * width + x] = simd::float2{x / (float) width, y / (float) height};
            }
        }
        bufferPixel = device->newBuffer(sizeof(simd::uint2) * width * height, MTL::ResourceStorageModeShared);
        memcpy(bufferPixel->contents(), pixelCoords, sizeof(simd::uint2) * width * height);

        auto *screenSize = new simd::uint2{width, height};
        bufferScreenSize = device->newBuffer(sizeof(simd::uint2), MTL::ResourceStorageModeShared);
        memcpy(bufferScreenSize->contents(), screenSize, sizeof(simd::uint2));

        bufferUV = device->newBuffer(sizeof(simd::float2) * width * height, MTL::ResourceStorageModeShared);
        memcpy(bufferUV->contents(), uvs, sizeof(simd::float2) * width * height);

        bufferResult = device->newBuffer(sizeof(simd::float4) * width * height, MTL::ResourceStorageModeShared);
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
        computeEncoder->setBuffer(bufferPixel, 0, 0);
        computeEncoder->setBuffer(bufferUV, 0, 1);
        computeEncoder->setBuffer(bufferResult, 0, 2);

        MTL::Size gridSize = MTL::Size::Make(width, height, 1);

        NS::UInteger threadGroupSize = variables->functionPSO->maxTotalThreadsPerThreadgroup();
        MTL::Size threadgroupSize = MTL::Size::Make(threadGroupSize, 1, 1);

        computeEncoder->dispatchThreads(gridSize, threadgroupSize);
    }

    Image *MetalRaytracer::outputBufferToImage() {
        auto *image = new Image(width, height);
        for (unsigned y = 0; y < height; ++y) {
            for (unsigned x = 0; x < width; ++x) {
                simd::float4 color = ((simd::float4 *) bufferResult->contents())[y * width + x];
                image->setPixel(x, y, Color::fromFloat4(color));
            }
        }
        return image;
    }


    Image *MetalRaytracer::raytrace(Scene scene) {
        auto *image = new Image(width, height);
        return image;
    }

    Image *MetalRaytracer::uvTest() {
        auto *variables = loadFunction("uvTest");
        sendComputeCommand(variables, &MetalRaytracer::encodeUVTestData);

        return outputBufferToImage();
    }
}
#endif
