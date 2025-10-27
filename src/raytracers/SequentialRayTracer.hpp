#pragma once
#include "../RayTracer.hpp"

namespace RayTracing {
    class SequentialRayTracer : public RayTracer {
    public:
        SequentialRayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        Image *raytrace(Scene scene) override;

        Image *uvTest() override;

        Image *rayTest(Camera *camera) override;

        std::string identifier() override { return "SequentialRayTracer"; }
    };
}