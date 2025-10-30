#pragma once
#include "../math/vectors.hpp"
#include "../math/matrices.hpp"
#ifdef USE_SHADER_METAL
#include "../../shader/metal/shader_types.hpp"
#endif

namespace RayTracing {
    /// Axis-aligned bounding box defined by two corner points
    struct BoundingBox {
        Vec3 minPos;
        Vec3 maxPos;

        BoundingBox() = default;

        /**
         * Creates a bounding box from two corner points
         * @param minPos The minimum corner point (smallest x, y, z).
         * @param maxPos The maximum corner point (largest x, y, z).
         */
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

        /// Returns the center point of the bounding box
        [[nodiscard]] Vec3 center() const {
            return size() / 2 + minPos;
        }

        /// Returns the size (width, height, depth) of the bounding box
        [[nodiscard]] Vec3 size() const {
            return maxPos - minPos;
        }

        /**
         * Checks if a point is inside the bounding box
         * @param p The point to check
         * @return True if the point is inside the bounding box, false otherwise
         */
        [[nodiscard]] bool contains(const Vec3 &p) const {
            return (p.getX() >= minPos.getX() && p.getX() <= maxPos.getX()) &&
                   (p.getY() >= minPos.getY() && p.getY() <= maxPos.getY()) &&
                   (p.getZ() >= minPos.getZ() && p.getZ() <= maxPos.getZ());
        }

#ifdef USE_SHADER_METAL
        Metal_BoundingBox toMetal() {
            return {minPos.toMetal(), maxPos.toMetal()};
        }
#endif
    };

    /// Nested bounding box for spatial partitioning
    struct NestedBoundingBox : public BoundingBox {
        /// containing the triangle indices list for this bounding box
        std::vector<int> indices;
        /// child left bounding box
        NestedBoundingBox *left;
        /// child right bounding box
        NestedBoundingBox *right;
        /// value at which the box is split
        float splitValue;
        /// axis along which the box is split
        Vec3::Direction splitAxis;

        /// Default constructor for easier initialization
        NestedBoundingBox() : BoundingBox(), left(nullptr), right(nullptr), splitValue(0), splitAxis(Vec3::X_AXIS) {
        }

        ~NestedBoundingBox() {
            delete left;
            delete right;
        }

        /**
         * Creates a nested bounding box for spatial partitioning
         * @param box parent bounding box
         * @param indices indices of objects contained in this box
         * @param left child left bounding box
         * @param right child right bounding box
         * @param split_value value at which the box is split
         * @param split_axis axis along which the box is split
         */
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

        /**
         * Gets the depth of the nested bounding box tree
         * @return max depth of the tree
         */
        [[nodiscard]] unsigned depth() const {
            if (left == nullptr && right == nullptr) {
                return 1;
            }
            unsigned leftDepth = left != nullptr ? left->depth() : 0;
            unsigned rightDepth = right != nullptr ? right->depth() : 0;
            return 1 + std::max(leftDepth, rightDepth);
        }

        std::vector<int> getAllIndices() const {
            std::vector<int> allIndices = indices;
            if (left != nullptr) {
                auto leftIndices = left->getAllIndices();
                allIndices.insert(allIndices.end(), leftIndices.begin(), leftIndices.end());
            }
            if (right != nullptr) {
                auto rightIndices = right->getAllIndices();
                allIndices.insert(allIndices.end(), rightIndices.begin(), rightIndices.end());
            }
            return allIndices;
        }
    };
}
