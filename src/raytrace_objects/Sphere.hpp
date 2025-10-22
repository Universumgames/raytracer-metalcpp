#pragma once
#include "RayTracableObject.hpp"

namespace RayTracing {
    struct SerializableSphere : public SerializableRayTraceableObject {
        float radius;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableSphere, position, color, radius)
    };

    class Sphere : public RayTraceableObject {
    public:
        float radius{};

        Sphere() : RayTraceableObject(RGBf{}, Vec3{}, {}) {
        }

        Sphere(const RGBf &color, const Vec3 &position,
               float radius)
            : RayTraceableObject(color, position, {}),
              radius(radius) {
        }

        Sphere(const SerializableSphere &obj) : RayTraceableObject(obj.color, obj.position, {}), radius(obj.radius) {
        }

        void updateBoundingBox() override {
            boundingBox = {
                {-radius, -radius, -radius},
                {radius, radius, radius}
            };
        }
    };
}