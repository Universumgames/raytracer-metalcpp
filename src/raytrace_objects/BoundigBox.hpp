#pragma once
#include "../math/vectors.hpp"
#include "../math/matrices.hpp"

namespace RayTracing {
    struct BoundingBox {
        Vec3 minPos;
        Vec3 maxPos;

        BoundingBox() = default;

        BoundingBox(const Vec3 &minPos, const Vec3 &maxPos) : minPos(minPos), maxPos(maxPos) {
        }

        [[nodiscard]] Vec3 right() const {
            return Vec3::right();
        }

        [[nodiscard]] Vec3 up() const {
            return Vec3::up();
        }

        [[nodiscard]] Vec3 forward() const {
            return Vec3::forward();
        }

        [[nodiscard]] Vec3 center() const {
            return size() / 2 + minPos;
        }

        [[nodiscard]] Vec3 size() const {
            return maxPos - minPos;
        }

        [[nodiscard]] bool contains(const Vec3 &p) const {
            return (p.getX() >= minPos.getX() && p.getX() <= maxPos.getX()) &&
                   (p.getY() >= minPos.getY() && p.getY() <= maxPos.getY()) &&
                   (p.getZ() >= minPos.getZ() && p.getZ() <= maxPos.getZ());
        }
    };

    struct NestedBoundingBox : public BoundingBox {
        std::vector<int> indices;
        NestedBoundingBox *left;
        NestedBoundingBox *right;
        float splitValue;
        Vec3::Direction splitAxis;

        NestedBoundingBox() {
        }

        NestedBoundingBox(const BoundingBox &box, const std::vector<int> &indices,
                          NestedBoundingBox *left, NestedBoundingBox *right, float split_value,
                          Vec3::Direction split_axis)
            : BoundingBox(box.minPos, box.maxPos),
              indices(indices),
              left(left),
              right(right),
              splitValue(split_value),
              splitAxis(split_axis) {
        }

        [[nodiscard]] unsigned depth() const {
            if (left == nullptr && right == nullptr) {
                return 1;
            }
            unsigned leftDepth = left != nullptr ? left->depth() : 0;
            unsigned rightDepth = right != nullptr ? right->depth() : 0;
            return 1 + std::max(leftDepth, rightDepth);
        }
    };
}