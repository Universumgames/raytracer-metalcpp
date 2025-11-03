#pragma once
#ifdef USE_SHADER_METAL
#include <Foundation/Foundation.hpp>
#include <Metal/MTLDevice.hpp>
#include "../RayTracer.hpp"
#include <Metal/MTLComputeCommandEncoder.hpp>

#include "../../shader/metal/shader_types.hpp"

namespace RayTracing {
    /// Ray tracer implementation using Apple's Metal API
    class MetalRaytracer final : public RayTracer {
    private:
        NS::AutoreleasePool *pAutoreleasePool = nullptr;
        MTL::Device *device = nullptr;
        MTL::CommandQueue *commandQueue = nullptr;
        MTL::Library *defaultLibrary = nullptr;

        MTL::Buffer *bufferScreenSize = nullptr;
        MTL::Buffer *bufferForward = nullptr;

        MTL::Buffer *bufferRayTraceSettings = nullptr;
        MTL::Buffer *bufferRays = nullptr;
        MTL::Buffer *bufferMeshObjects = nullptr;
        MTL::Buffer *bufferMeshVertices = nullptr;
        MTL::Buffer *bufferNormals = nullptr;
        MTL::Buffer *bufferMeshIndices = nullptr;
        MTL::Buffer *bufferBoundingBoxes = nullptr;
        MTL::Buffer *bufferSphereObjects = nullptr;
        MTL::Buffer *bufferLights = nullptr;

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
            Scene *scene = nullptr;
        };

        void sendComputeCommand(MetalEncodingData data,
                                void (MetalRaytracer::*function)(MetalEncodingData,
                                                                 MTL::ComputeCommandEncoder *));

        void encodeUVTestData(MetalEncodingData, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRaytracingData(MetalEncodingData, MTL::ComputeCommandEncoder *computeEncoder);

        void encodeRayTestData(MetalEncodingData, MTL::ComputeCommandEncoder *computeEncoder);

        Image *outputBufferToImage(unsigned samples);

        static std::vector<Metal_Ray> raysToMetal(const std::vector<Ray> &rays);

        struct Metal_MeshTransformationReturn {
            std::vector<Metal_MeshRayTraceableObject> meshObjects;
            std::vector<simd::float3> vertices;
            std::vector<int> indices;
            std::vector<simd::float3> normals;
            std::vector<Metal_NestedBoundingBox> boundingBoxes;
        };

        static Metal_MeshTransformationReturn
        meshObjectsToMetal(const std::vector<MeshedRayTraceableObject *> &objects);

        static std::vector<Metal_SphereRayTraceableObject> sphereObjectsToMetal(
            const std::vector<SphereRayTraceableObject *> &objects);

        static std::vector<Metal_Light> lightsToMetal(const std::vector<LightSource *> &lights);

    public:
        MetalRaytracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        ~MetalRaytracer() override;

        /**
         * Raytrace a scene and generate image
         * @param scene scene to raytrace
         * @return raytraced image
         */
        Image *raytrace(Scene scene) override;

        /**
         * Simple UV Space image test, used to test basic compute pipeline
         * @return uv image
         */
        Image *uvTest() override;

        /**
         * Simple ray test to check ray generation
         * @param camera camera to generate rays from
         * @return image with ray directions encoded as colors
         */
        Image *rayTest(Camera *camera) override;

        /// Get the identifier of the raytracer
        std::string identifier() override { return "MetalRayTracer"; }
    };
}


#endif