#pragma once
#ifdef USE_SHADER_METAL
#include <Foundation/Foundation.hpp>
#include <Metal/MTLDevice.hpp>
#include "../RayTracing.hpp"
#include <Metal/MTLComputeCommandEncoder.hpp>

namespace RayTracing {
    class MetalRaytracer : public RayTracer {
    private:
        NS::AutoreleasePool *pAutoreleasePool = nullptr;
        MTL::Device *device = nullptr;
        MTL::CommandQueue *commandQueue = nullptr;
        MTL::Library *defaultLibrary = nullptr;

        MTL::Buffer* bufferPixel = nullptr;
        MTL::Buffer* bufferScreenSize = nullptr;
        MTL::Buffer* bufferUV = nullptr;

        MTL::Buffer* bufferResult = nullptr;

    private:
        void initialize();

        struct KernelFunctionVariables {
            MTL::Function *function = nullptr;
            MTL::ComputePipelineState *functionPSO = nullptr;
        };

        KernelFunctionVariables *loadFunction(const std::string &name);

        void sendComputeCommand(KernelFunctionVariables *variables,
                                void (MetalRaytracer::*function)(KernelFunctionVariables *,
                                                                 MTL::ComputeCommandEncoder *));

        void encodeUVTestData(KernelFunctionVariables *, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRaytracingData(KernelFunctionVariables *, MTL::ComputeCommandEncoder *computeEncoder);

        Image* outputBufferToImage();

    public:
        MetalRaytracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel);

        ~MetalRaytracer() override;

        Image *raytrace(Scene scene) override;

        Image *uvTest() override;

        std::string identifier() override { return "MetalRaytracer"; }
    };
}


#endif
