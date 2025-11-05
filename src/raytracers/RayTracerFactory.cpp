#include "RayTracerFactory.hpp"

#include "CudaRayTracer.hpp"
#include "MetalRaytracer.hpp"
#include "OpenMPRayTracer.hpp"
#include "SequentialRayTracer.hpp"

namespace RayTracing {
    RayTracerFactory *RayTracerFactory::instance = nullptr;

    RayTracerFactory *RayTracerFactory::init(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel) {
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

    RayTracerFactory::RayTracerFactory(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel) {
        this->sequentialRayTracer = new SequentialRayTracer(windowSize, bounces,
                                                            samplesPerPixel);

        this->multiThreadedRayTracer = new OpenMPRayTracer(windowSize, bounces, samplesPerPixel);

#ifdef USE_SHADER_METAL
        this->shaderRayTracer = new MetalRaytracer(windowSize, bounces, samplesPerPixel);
#endif
#ifdef USE_SHADER_CUDA
        this->shaderRayTracer = new CudaRayTracer(windowSize, bounces, samplesPerPixel);
#endif
    }
}
