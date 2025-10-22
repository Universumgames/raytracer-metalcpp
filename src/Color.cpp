#include "Color.hpp"

#include <iostream>

#include "vectors.hpp"

namespace RayTracing {

    RGBA8 RGBA8::avg(std::vector<RGBA8> colors) {
        if (colors.empty()) {
            return {};
        }
        if (colors.size() == 1) {
            return colors[0];
        }
        Vec4 avgColor = colors[0].forceVec4();
        for (int i = 1; i < colors.size(); i++) {
            avgColor += colors[i].forceVec4();
        }
        return RGBA8::forceFromVec4(avgColor / ((float)colors.size()));
    }

    RGBf RGBf::blend(const std::vector<RGBf> &colors, ColorBlendMode mode) {
        if (colors.empty()) {
            return {1,0,0,0};
        }
        switch (mode) {
            case AVERAGE: {
                RGBf sum{0,0,0,0};
                for (auto & color : colors) {
                    sum += color;
                }
                return sum / colors.size();
            }
            default: return {1,0,0,0};
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
