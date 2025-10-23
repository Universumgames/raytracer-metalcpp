#pragma once
#include "../RayTracing.hpp"

namespace RayTracing {
    class RayTracerFactory {
    private:
        static RayTracerFactory *instance;

        RayTracer *sequentialRayTracer = nullptr;
        RayTracer *parallelRayTracer = nullptr;

        RayTracerFactory(const Vec2 &windowSize, int bounces, int samplesPerPixel);

    public:
        static void init(const Vec2 &windowSize, int bounces, int samplesPerPixel);

        static RayTracerFactory *getInstance();

        /// get the Sequential Raytracer
        RayTracer *getSequentialImplementation() { return sequentialRayTracer; }

        /// get the supported GPU raytracer
        RayTracer *getParallelImplementation() { return parallelRayTracer; }
    };
}