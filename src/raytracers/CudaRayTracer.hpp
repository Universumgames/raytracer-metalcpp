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

        std::string identifier() override { return "CudaRayTracer"; }

        Image *raytrace(Scene scene) override;

        Image *uvTest() override;

        Image *rayTest(Camera *camera) override;
    };
}
#endif