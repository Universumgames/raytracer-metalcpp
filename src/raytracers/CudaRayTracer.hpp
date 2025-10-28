#pragma once
#ifdef USE_SHADER_CUDA
#include "../RayTracing.hpp"

namespace RayTracing {
    class CudaRayTracer : public RayTracer {
    private:

    public:
        CudaRayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel)
            : RayTracer(width, height, bounces, samplesPerPixel) {
        }

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