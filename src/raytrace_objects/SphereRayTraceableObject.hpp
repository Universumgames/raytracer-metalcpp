#pragma once
#include "RayTracableObject.hpp"

namespace RayTracing {
    /// Serializable representation of a sphere ray traceable object
    struct SerializableSphere : public SerializableRayTraceableObject {
        float radius{};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableSphere, position, color, specularIntensity, radius)
    };

    /// Ray traceable object represented by a sphere
    class SphereRayTraceableObject : public RayTraceableObject {
    public:
        float radius{};

        SphereRayTraceableObject() : RayTraceableObject(RGBf{}, 1, Vec3{}, {1, 1, 1}, {}) {
        }

        SphereRayTraceableObject(const RGBf &color, float specularIntensity, const Vec3 &position,
                                 float radius)
            : RayTraceableObject(color, specularIntensity, position, {1, 1, 1}, {}),
              radius(radius) {
        }

        SphereRayTraceableObject(const SerializableSphere &obj) : RayTraceableObject(obj.color, obj.specularIntensity,
                                                                      obj.position, {1, 1, 1},
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