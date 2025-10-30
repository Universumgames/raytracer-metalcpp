#ifdef USE_SHADER_CUDA
#include "CudaRayTracer.hpp"
#include <cuda_runtime.h>
#include <nvrtc_helper.h>
#include <helper_functions.h>

namespace RayTracing {

    CudaRayTracer::CudaRayTracer(const Vec2u& screenSize, unsigned bounces, unsigned samplesPerPixel)
            : RayTracer(screenSize, bounces, samplesPerPixel) {
    }

    Image *CudaRayTracer::raytrace(Scene scene) {
        auto *image = new Image(getWindowSize());
        return image;
    }

    Image *CudaRayTracer::uvTest() {
        auto *image = new Image(getWindowSize());
        return image;
    }

    Image *CudaRayTracer::rayTest(Camera *camera) {
        auto *image = new Image(getWindowSize());
        return image;
    }
}
#endif