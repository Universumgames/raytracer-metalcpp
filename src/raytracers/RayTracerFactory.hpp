#pragma once
#include "../RayTracer.hpp"

namespace RayTracing {
    /// Factory class to create and manage different RayTracer implementations
    class RayTracerFactory {
    private:
        static RayTracerFactory *instance;

        RayTracer *sequentialRayTracer = nullptr;
        RayTracer *shaderRayTracer = nullptr;

        RayTracerFactory(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

    public:
        ~RayTracerFactory() = delete;

        /**
         * Initialize the RayTracerFactory singleton
         * @param windowSize desired window size
         * @param bounces bounce count
         * @param samplesPerPixel samples per pixel
         * @return pointer to the RayTracerFactory instance
         */
        static RayTracerFactory *init(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        /// Get the RayTracerFactory singleton instance
        static RayTracerFactory *getInstance();

        /// get the Sequential Raytracer
        RayTracer *getSequentialImplementation() { return sequentialRayTracer; }

        /// get the supported GPU raytracer
        RayTracer *getShaderImplementation() { return shaderRayTracer; }
    };
}