#include "RayTracerFactory.hpp"

#include "CudaRayTracer.hpp"
#include "MetalRaytracer.hpp"
#include "SequentialRayTracer.hpp"

namespace RayTracing {
    RayTracerFactory *RayTracerFactory::instance = nullptr;

    RayTracerFactory *RayTracerFactory::init(const Vec2u &windowSize, int bounces, int samplesPerPixel) {
        if (instance != nullptr) {
            return instance;
        }
        instance = new RayTracerFactory(windowSize, bounces, samplesPerPixel);
        return instance;
    }

    RayTracerFactory *RayTracerFactory::getInstance() {
        if (instance == nullptr) {
            throw std::runtime_error("RayTracerFactory was not initialized");
        }
        return instance;
    }

    RayTracerFactory::RayTracerFactory(const Vec2u &windowSize, int bounces, int samplesPerPixel) {
        this->sequentialRayTracer = new SequentialRayTracer(windowSize, bounces,
                                                            samplesPerPixel);
#ifdef USE_SHADER_METAL
        this->parallelRayTracer = new MetalRaytracer(windowSize, bounces, samplesPerPixel);
#elifdef USE_SHADER_CUDA
        this->sequentialRayTracer = new CudaRayTracer(windowSize, bounces, samplesPerPixel);
#endif
    }
}