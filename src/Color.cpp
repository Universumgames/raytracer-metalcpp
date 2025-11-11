#include "Color.hpp"

#include "math/vectors.hpp"

namespace RayTracing {
    RGBf RGBf::blend(const std::vector<RGBf> &colors, ColorBlendMode mode) {
        return blend(colors.data(), colors.size(), mode);
    }

    RGBf RGBf::blend(const RGBf *colors, size_t count, ColorBlendMode mode) {
        if (count == 0) {
            return RGBf::BLACK();
        }
        switch (mode) {
            case AVERAGE: {
                RGBf sum{0, 0, 0, 0};
                for (int i = 0; i < count; i++) {
                    sum += colors[i];
                }
                return sum / (float) count;
            }
            case COUNT: {
                return {1.f / (float) count, 1.f / (float) count, 1.f / (float) count, 1.f};
            }
            default: return RGBf::BLACK();
        }
    }


    RGBf RGBf::operator*=(const RGBf &other) {
        this->a() *= other.getA();
        this->r() *= other.getR();
        this->g() *= other.getG();
        this->b() *= other.getB();

        return *this;
    }

    RGBA8 RGBf::toRGBA8() const {
        return {
            getR() * 255.f,
            getG() * 255.f,
            getB() * 255.f,
            getA() * 255.f
        };
    }
}