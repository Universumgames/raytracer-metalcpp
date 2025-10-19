#pragma once
#include <cmath>
#include <vector>
#include <simd/vector_types.h>

#include "SFML/Graphics/Color.hpp"

namespace RayTracing {
    struct Point {
        int x;
        int y;
    };

    struct Color {
        uint8_t r, g, b;

        [[nodiscard]] sf::Color toSFMLColor() const {
            return {r, g, b};
        }

        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
        }

        Color(double r, double g, double b) : r(floor(r)), g(floor(g)), b(floor(b)) {
        }

        Color(unsigned r, unsigned g, unsigned b) : r(r), g(g), b(b) {
        }

#ifdef USE_SHADER_METAL
        static Color fromFloat4(simd::float4 f) {
            return {f[0] * 255, f[1] * 255, f[2] * 255};
        }
#endif
    };

    struct Ray {
        Point origin;
        Point direction;
        std::vector<Color> colors;
    };

    class Image {
    private:
        unsigned int width, height;

        Color **image;

    public:
        Image() = delete;

        Image(unsigned int width, unsigned int height);

        void setPixel(unsigned int x, unsigned int y, Color color);

        Color getPixel(unsigned int x, unsigned int y);

        Color *valueAt(unsigned int x, unsigned int y);
    };
};
