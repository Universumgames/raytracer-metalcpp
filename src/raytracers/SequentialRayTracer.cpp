#include "SequentialRayTracer.hpp"

#include "SFML/System/Vector2.hpp"

namespace RayTracing {
    SequentialRayTracer::SequentialRayTracer(unsigned width, unsigned height, unsigned bounces,
                                             unsigned samplesPerPixel) : RayTracer(
        width, height, bounces, samplesPerPixel) {
    }

    Image *SequentialRayTracer::uvTest() {
        auto *image = new Image(width, height);

        for (unsigned y = 0; y < height; y++) {
            for (unsigned x = 0; x < width; x++) {
                sf::Vector2 uv = {y / (double) height, x / (double) width};
                image->setPixel(x, y, {uv.x * 255, uv.y * 255, 0});
            }
        }

        return image;
    }


    Image *SequentialRayTracer::raytrace(Scene scene) {
        auto *image = new Image(width, height);
        auto rays = calculateStartingRays();

        return image;
    }
}
