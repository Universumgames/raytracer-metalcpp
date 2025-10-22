#pragma once
#include "../vectors.hpp"
#include "../matrices.hpp"

namespace RayTracing {
    struct BoundingBox {
        Vec3 minPos;
        Vec3 maxPos;

        BoundingBox() = default;

        BoundingBox(const Vec3 &minPos, const Vec3 &maxPos) : minPos(minPos), maxPos(maxPos) {
        }

        BoundingBox transformedRotated(Mat3x3 rotation, Vec3 translation);

        Vec3 right() const {
            return Vec3::right();
        }
        Vec3 up() const {
            return Vec3::up();
        }
        Vec3 forward() const {
            return Vec3::forward();
        }

        Vec3 center() const {
            return size() / 2 + minPos;
        }

        Vec3 size() const {
            return maxPos - minPos;
        }
    };
}