#include "CudaRayTracer.hpp"
#ifdef USE_SHADER_CUDA

namespace RayTracing {
    Image *CudaRayTracer::raytrace(Scene scene) {
        auto *image = new Image(width, height);
        return image;
    }

    Image *CudaRayTracer::uvTest() {
        auto *image = new Image(width, height);
        return image;
    }

    Image *CudaRayTracer::rayTest(Camera *camera) {
        auto *image = new Image(width, height);
        return image;
    }
}
#endif