#pragma once
#include "Transform.hpp"

namespace RayTracing {
    /// Serializable representation of a camera
    struct SerializableCamera {
        Vec3 position;
        Vec3 direction;
        float fov;
        Vec2 size;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableCamera, position, direction, fov, size)
    };

    /// Camera class representing a viewpoint in 3D space
    class Camera {
    public:
        Transform transform;
        float fov;
        Vec2 size;

        /**
         * Creates a Camera with the given position, direction, field of view, and size
         * @param position position of the camera
         * @param direction direction the camera is facing
         * @param fov field of view of the camera
         * @param size size of the camera viewport
         */
        Camera(Vec3 position, Vec3 direction, float fov, Vec2 size) : transform({
                                                                          position, direction
                                                                      }), fov(fov), size(size) {
        }

        /// Creates a Camera from a SerializableCamera
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