#pragma once
#include "RayTracableObject.hpp"

namespace RayTracing {
    /// Serializable representation of a sphere ray traceable object
    struct SerializableSphere : public SerializableRayTraceableObject {
        float radius{};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableSphere, position, color, radius)
    };

    /// Ray traceable object represented by a sphere
    class SphereRayTraceableObject : public RayTraceableObject {
    public:
        float radius{};

        SphereRayTraceableObject() : RayTraceableObject(RGBf{}, Vec3{}, {1, 1, 1}, {}) {
        }

        SphereRayTraceableObject(const RGBf &color, const Vec3 &position,
                                 float radius)
            : RayTraceableObject(color, position, {1, 1, 1}, {}),
              radius(radius) {
        }

        SphereRayTraceableObject(const SerializableSphere &obj) : RayTraceableObject(obj.color, obj.position, {1, 1, 1},
                                                                      {}),
                                                                  radius(obj.radius) {
        }

        void updateBoundingBox() override {
            boundingBox = {
                {-radius, -radius, -radius},
                {radius, radius, radius}
            };
        }
    };
}