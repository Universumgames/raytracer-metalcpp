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

        MTL::Buffer *bufferPixel = nullptr;
        MTL::Buffer *bufferScreenSize = nullptr;
        MTL::Buffer *bufferUV = nullptr;
        MTL::Buffer *bufferForward = nullptr;
        MTL::Buffer *bufferRays = nullptr;
        MTL::Buffer *bufferRayTraceSettings = nullptr;

        MTL::Buffer *bufferResult = nullptr;

    private:
        void initialize();

        struct KernelFunctionVariables {
            MTL::Function *function = nullptr;
            MTL::ComputePipelineState *functionPSO = nullptr;
        };

        KernelFunctionVariables loadFunction(const std::string &name);

        struct MetalEncodingData {
            KernelFunctionVariables variables;
            Scene *scene;
        };

        void sendComputeCommand(MetalEncodingData data,
                                void (MetalRaytracer::*function)(MetalEncodingData,
                                                                 MTL::ComputeCommandEncoder *));

        void encodeUVTestData(MetalEncodingData, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRaytracingData(MetalEncodingData, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRayTestData(MetalEncodingData, MTL::ComputeCommandEncoder *computeEncoder);

        Image *outputBufferToImage();

        static std::vector<Metal_Ray> raysToMetal(const std::vector<Ray> &rays);

        struct Metal_MeshTransformationReturn {
            std::vector<Metal_MeshRayTraceableObject> meshObjects;
            std::vector<simd::float3> vertices;
            std::vector<float> indices;
        };

        static Metal_MeshTransformationReturn
        meshObjectsToMetal(const std::vector<MeshedRayTraceableObject *> &objects);

        static std::vector<Metal_SphereRayTraceableObject> sphereObjectsToMetal(
            const std::vector<SphereRayTraceableObject *> &objects);

        static std::vector<Metal_Light> lightsToMetal(const std::vector<LightSource *> &lights);

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
