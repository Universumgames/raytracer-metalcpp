#pragma once
#include "RayTracableObject.hpp"

namespace RayTracing {
    struct SerializableSphere: public SerializableRayTraceableObject {
        float radius;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableSphere, position, color, radius)
    };
    class Sphere : public RayTraceableObject {
    public:
        float radius{};

        Sphere() : RayTraceableObject(RGBf{}, Vec3{}, Quaternion{}) {
        }

        Sphere(const RGBf &color, const Vec3 &position, const Quaternion &quaternion,
               float radius)
            : RayTraceableObject(color, position, quaternion),
              radius(radius) {
        }

        Sphere(const SerializableSphere &obj): RayTraceableObject(obj.color, obj.position, Quaternion{}), radius(obj.radius) {}

        void updateBoundingBox() override {
            boundingBox = {
                {-radius, -radius, -radius},
                {radius, radius, radius}
            };
        }
    };
}
