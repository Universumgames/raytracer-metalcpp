#include "RayTracing.hpp"

namespace RayTracing {
    RayTracer::RayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel) {
        this->width = width;
        this->height = height;
        this->bounces = bounces;
        this->samplesPerPixel = samplesPerPixel;
    }

    RayTracer::~RayTracer() {
    }
}
