#pragma once
#include "../RayTracer.hpp"

namespace RayTracing {
    class SequentialRayTracer : public RayTracer {
    public:
        SequentialRayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

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
        std::string identifier() override { return "SequentialRayTracer"; }
    };
}