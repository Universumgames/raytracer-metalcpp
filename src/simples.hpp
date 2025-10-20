#pragma once
#include <cmath>
#include <vector>
#include <nlohmann/json.hpp>
#include <simd/vector_types.h>

#include "SFML/Graphics/Color.hpp"

#include "vectors.hpp"

namespace RayTracing {

    struct Color {
        uint8_t r, g, b, a;

        [[nodiscard]] sf::Color toSFMLColor() const {
            return {r, g, b};
        }

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {
        }

        Color(double r, double g, double b, double a = 255) : r(floor(r)), g(floor(g)), b(floor(b)), a(floor(a)) {
        }

        Color(unsigned r, unsigned g, unsigned b, unsigned a = 255) : r(r), g(g), b(b), a(a) {
        }

        Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Color, r, g, b, a)

#ifdef USE_SHADER_METAL
        static Color fromFloat4(simd::float4 f) {
            return {f[0] * 255, f[1] * 255, f[2] * 255};
        }
#endif
    };

    struct Ray {
        Vec3 origin;
        Vec3 direction;
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
