#pragma once
#include <cmath>
#include <vector>
#include <nlohmann/json.hpp>

#include "SFML/Graphics/Color.hpp"

#ifdef USE_SHADER_METAL
#include <simd/vector_types.h>
#endif

namespace RayTracing {
    struct Color {
        uint8_t r, g, b, a;

        [[nodiscard]] sf::Color toSFMLColor() const {
            return {r, g, b};
        }

        Color &operator+=(const Color& value) {
            r += value.r;
            g += value.g;
            b += value.b;
            return *this;
        }

        Color(): r(0), g(0), b(0), a(0) {}

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

        Color(double r, double g, double b, double a = 255) : r(floor(r)), g(floor(g)), b(floor(b)), a(floor(a)) {}

        Color(unsigned r, unsigned g, unsigned b, unsigned a = 255) : r(r), g(g), b(b), a(a) {}

        Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Color, r, g, b, a)

        static Color avg(std::vector<Color> colors);

#ifdef USE_SHADER_METAL
        static Color fromFloat4(simd::float4 f) {
            return {f[0] * 255, f[1] * 255, f[2] * 255};
        }
#endif

    };

}
