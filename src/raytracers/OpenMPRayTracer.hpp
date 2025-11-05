#pragma once
#include "SequentialRayTracer.hpp"

namespace RayTracing {
    /// Raytracer implementation using OpenMP for parallelization on CPU
    class OpenMPRayTracer : public SequentialRayTracer {
    public:
        OpenMPRayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        /**
         * Raytrace a scene and generate image
         * @param scene scene to raytrace
         * @return raytraced image
         */
        Image *raytrace(Scene scene) override;

        /// Get the identifier of the raytracer
        std::string identifier() override {
            return "OpenMPRaytracer";
        }
    };
}
