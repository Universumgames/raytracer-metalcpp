#pragma once
#ifdef USE_SHADER_METAL
#include <Foundation/Foundation.hpp>
#include <Metal/MTLDevice.hpp>
#include "../RayTracer.hpp"
#include <Metal/MTLComputeCommandEncoder.hpp>

#include "../../shader/metal/shader_types.hpp"

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
        MTL::Buffer *bufferForward = nullptr;
        MTL::Buffer *bufferRays = nullptr;

        MTL::Buffer* bufferResult = nullptr;

    private:
        void initialize();

        struct KernelFunctionVariables {
            MTL::Function *function = nullptr;
            MTL::ComputePipelineState *functionPSO = nullptr;
            std::vector<Metal_Ray> *metalRays = nullptr;
        };

        KernelFunctionVariables *loadFunction(const std::string &name);

        void sendComputeCommand(KernelFunctionVariables *variables,
                                void (MetalRaytracer::*function)(KernelFunctionVariables *,
                                                                 MTL::ComputeCommandEncoder *));

        void encodeUVTestData(KernelFunctionVariables *, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRaytracingData(KernelFunctionVariables *, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRayTestData(KernelFunctionVariables *, MTL::ComputeCommandEncoder *computeEncoder);

        Image* outputBufferToImage();

        static std::vector<Metal_Ray> raysToMetal(const std::vector<Ray> &rays);

    public:
        MetalRaytracer(Vec2u windowSize, unsigned bounces, unsigned samplesPerPixel);

        ~MetalRaytracer() override;

        Image *raytrace(Scene scene) override;

        Image *uvTest() override;

        Image *rayTest(Camera *camera) override;

        std::string identifier() override { return "MetalRaytracer"; }
    };
}


#endif
