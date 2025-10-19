#pragma once
#include "Scene.hpp"
#include "simples.hpp"

namespace RayTracing {
    class RayTracer {
    protected:
        unsigned int width, height;
        unsigned bounces;
        unsigned samplesPerPixel;

    public:
        RayTracer() = delete;

        RayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel);

        virtual ~RayTracer();

        virtual Image *raytrace(Scene scene) = 0;

        virtual Image *uvTest() = 0;
    };
}
