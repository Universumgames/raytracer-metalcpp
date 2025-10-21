#pragma once
#include "../RayTracing.hpp"

namespace RayTracing {
    class SequentialRayTracer : public RayTracer {
    public:
        SequentialRayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel);

        Image *raytrace(Scene scene) override;

        Image *uvTest() override;

        std::string identifier() override { return "SequentialRayTracer"; }
    };
}
