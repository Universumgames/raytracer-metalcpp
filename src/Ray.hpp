#pragma once
#include "Color.hpp"
#include "raytrace_objects/RayTracableObject.hpp"
#include "math/vectors.hpp"

namespace RayTracing {
    struct LocalRay;

    /// Information about a ray-object intersection
    struct HitInfo {
        bool hit;
        Vec3 hitPoint;
        Vec3 normal;
        float distance;
        bool isLight = false;
    };

    /// Represents a ray in 3D space
    struct Ray {
        Vec3 origin;
        Vec3 direction;
        Vec3 rngSeed; /// Seed for random number generation, since shaders don't have good random functions
        std::vector<RGBf> colors;

        unsigned idX, idY;

        RGBf lightColor{0, 0, 0, 0};
        float totalDistance = 0;

        /**
         * Reflect this ray at a given location with a normal vector
         * @param location the location to reflect at
         * @param normal the normal vector at the location
         * @param totalReflection the total reflection coefficient (0.0 to 1.0)
         * @return the reflected direction vector
         */
        Vec3 reflectAt(const Vec3 &location, const Vec3 &normal, float totalReflection = 0.9);

        /**
         * Convert this ray to local object space using the given transform
         * @param transform the transform to apply
         * @return the ray in local object space
         */
        [[nodiscard]] LocalRay toLocalRay(const Transform &transform) const;

        /**
         * Calculate hit information of sphere
         * @param sphereCenter center of the sphere
         * @param sphereRadius radius of the sphere
         * @return intersection point on sphere
         */
        [[nodiscard]] HitInfo intersectSphere(const Vec3 &sphereCenter, float sphereRadius) const;
    };

    /// Represents a ray in local object space
    struct LocalRay : public Ray {
        /**
         * Calculate hit information of triangle
         * @param triangle points of the triangle
         * @param customNormal
         * @return intersection point on triangle
         */
        [[nodiscard]] HitInfo intersectTriangle(Vec3 triangle[], const Vec3 &customNormal) const;

        /**
         * Check if this ray intersects with a bounding box
         * @param box the bounding box to check against
         * @return true if the ray intersects the bounding box, false otherwise
         */
        [[nodiscard]] bool intersectsBoundingBox(const BoundingBox &box) const;
    };
}