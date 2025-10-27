#pragma once
#include "../RayTracer.hpp"

namespace RayTracing {
    class RayTracerFactory {
    private:
        static RayTracerFactory *instance;

        RayTracer *sequentialRayTracer = nullptr;
        RayTracer *shaderRayTracer = nullptr;

        RayTracerFactory(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

    public:
        ~RayTracerFactory() = delete;

        static RayTracerFactory *init(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        static RayTracerFactory *getInstance();

        /// get the Sequential Raytracer
        RayTracer *getSequentialImplementation() { return sequentialRayTracer; }

        /// get the supported GPU raytracer
        RayTracer *getShaderImplementation() { return shaderRayTracer; }
    };
}