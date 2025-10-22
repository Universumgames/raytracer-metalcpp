#pragma once
#include "Sphere.hpp"

namespace RayTracing {
    struct SerializableLightSource : public SerializableSphere {
        RGBA8 emittingColor;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableLightSource, position, radius, emittingColor)
    };

    class LightSource : public Sphere {
    public:
        RGBf emittingColor{};

        LightSource() : Sphere(RGBf{}, Vec3{}, 0) {
        }

        LightSource(const RGBf &color, const Vec3 &position,
                    float radius, const RGBf &emitting_color)
            : Sphere(color, position, radius),
              emittingColor(emitting_color) {
        }

        LightSource(const SerializableLightSource &obj) : Sphere({}, obj.position, obj.radius),
                                                          emittingColor(obj.emittingColor) {
        }
    };
}