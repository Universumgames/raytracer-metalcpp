#pragma once
#include <cmath>
#include <vector>
#include <nlohmann/json.hpp>

#include "math/vectors.hpp"
#include "SFML/Graphics/Color.hpp"

#ifdef USE_SHADER_METAL
#include <simd/vector_types.h>
#endif

namespace RayTracing {
    struct RGBA8 {
        uint8_t r, g, b, a;

        [[nodiscard]] sf::Color toSFMLColor() const {
            return {r, g, b};
        }

        RGBA8 &operator+=(const RGBA8 &value) {
            r += value.r;
            g += value.g;
            b += value.b;
            return *this;
        }

        RGBA8() : r(0), g(0), b(0), a(0) {
        }

        RGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {
        }

        RGBA8(double r, double g, double b, double a = 255) : r(floor(r)), g(floor(g)), b(floor(b)), a(floor(a)) {
        }

        RGBA8(unsigned r, unsigned g, unsigned b, unsigned a = 255) : r(r), g(g), b(b), a(a) {
        }

        RGBA8(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RGBA8, r, g, b, a)

        /// warning this method should be used as confusion with the float-valued color encoding can occur
        [[nodiscard]] Vec4 forceVec4

        (
        )
        const
 {
            return Vec4{static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a)};
        }

        /// warning this method should be used as confusion with the float-valued color encoding can occur
        static RGBA8 forceFromVec4(Vec4 color) {
            return {color.x(), color.y(), color.z(), color.w()};
        }

#ifdef USE_SHADER_METAL
        static RGBA8 fromFloat4(simd::float4 f) {
            return {f[0] * 255, f[1] * 255, f[2] * 255, f[3] * 255};
        }
#endif
    };

    enum ColorBlendMode {
        AVERAGE,
        COUNT
    };

    struct RGBf : public Vec4 {
    public:
        RGBf() : Vec4(0, 0, 0, 0) {
        }

        RGBf(float r, float g, float b, float a) : Vec4(r, g, b, a) {
        }

        RGBf(const RGBf &other) = default;

        RGBf(const Vec4 &other) : Vec4(other) {
        }

        RGBf(const RGBA8 &other) : Vec4((float) other.r / 255.f, (float) other.g / 255.f, (float) other.b / 255.f,
                                        (float) other.a / 255.f) {
        }

        [[nodiscard]] float getA() const { return this->getW(); }
        [[nodiscard]] float getR() const { return this->getX(); }
        [[nodiscard]] float getG() const { return this->getY(); }
        [[nodiscard]] float getB() const { return this->getZ(); }

        float &a() { return this->w(); }
        float &r() { return this->x(); }
        float &g() { return this->y(); }
        float &b() { return this->z(); }

        static RGBf blend(const std::vector<RGBf> &colors, ColorBlendMode mode = ColorBlendMode::AVERAGE);

        RGBf operator*=(const RGBf &other);

        [[nodiscard]] RGBA8 toRGBA8() const;

        static RGBf BLACK() { return {0, 0, 0, 1}; }

#ifdef USE_SHADER_METAL
        static RGBf fromFloat4(simd::float4 f) {
            return {f[0], f[1], f[2], f[3]};
        }
#endif
    };
}