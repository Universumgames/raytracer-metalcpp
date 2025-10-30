#pragma once
#ifdef USE_SHADER_CUDA
#include "../RayTracer.hpp"
#include "shader_types.cuh"

namespace RayTracing {
    class CudaRayTracer final : public RayTracer {
    private:

        struct Cuda_Ray {

        };

        Image *outputBufferToImage(unsigned samples);

        cudaDeviceProp deviceProp = {};

        RGBf* bufferResult;

    public:
        CudaRayTracer(const Vec2u& screenSize, unsigned bounces, unsigned samplesPerPixel);

        ~CudaRayTracer() override = default;

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
        std::string identifier() override { return "CudaRayTracer"; }
    };
}
#endif