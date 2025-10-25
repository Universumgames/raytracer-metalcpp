#pragma once
#include "SphereRayTraceableObject.hpp"

namespace RayTracing {
    struct SerializableLightSource : public SerializableSphere {
        RGBA8 emittingColor;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableLightSource, position, radius, emittingColor)
    };

    class LightSource : public SphereRayTraceableObject {
    public:
        RGBf emittingColor{};

        LightSource() : SphereRayTraceableObject(RGBf{}, Vec3{}, 0) {
        }

        LightSource(const RGBf &color, const Vec3 &position,
                    float radius, const RGBf &emitting_color)
            : SphereRayTraceableObject(color, position, radius),
              emittingColor(emitting_color) {
        }

        LightSource(const SerializableLightSource &obj) : SphereRayTraceableObject({}, obj.position, obj.radius),
                                                          emittingColor(obj.emittingColor) {
        }
    };
}