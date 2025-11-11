#pragma once
#include "SphereRayTraceableObject.hpp"

namespace RayTracing {
    /// Serializable representation of a light source
    struct SerializableLightSource : public SerializableSphere {
        RGBA8 emittingColor;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableLightSource, position, radius, emittingColor)
    };

    /// Light source represented as a sphere that emits light
    class LightSource : public SphereRayTraceableObject {
    public:
        RGBf emittingColor{};

        LightSource() : SphereRayTraceableObject(RGBf{}, 1, Vec3{}, 0) {
        }

        LightSource(const RGBf &color, const Vec3 &position,
                    float radius, const RGBf &emitting_color)
            : SphereRayTraceableObject(color, 1, position, radius),
              emittingColor(emitting_color) {
        }

        explicit LightSource(const SerializableLightSource &obj) : SphereRayTraceableObject(
                                                                       {}, 1, obj.position, obj.radius),
                                                                   emittingColor(obj.emittingColor) {
        }
    };
}