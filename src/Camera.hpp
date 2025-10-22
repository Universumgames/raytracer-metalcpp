#pragma once
#include "Transform.hpp"

namespace RayTracing {
    struct SerializableCamera {
        Vec3 position;
        Vec3 direction;
        float fov;
        Vec2 size;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableCamera, position, direction, fov, size)
    };

    class Camera {
    public:
        Transform transform;
        float fov;
        Vec2 size;

        Camera(Vec3 position, Vec3 direction, float fov, Vec2 size) : transform({
                                                                          position, direction
                                                                      }), fov(fov), size(size) {
        }

        Camera(const SerializableCamera &cam) : transform({cam.position, cam.direction}),
                                                fov(cam.fov), size(cam.size) {
        }

        Vec3 up() {
            return Vec3::up();
        }

        Vec3 forward() {
            return Vec3::forward();
        }

        Vec3 right() {
            return Vec3::right();
        }
    };
}