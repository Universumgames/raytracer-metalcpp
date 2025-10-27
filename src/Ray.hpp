#pragma once
#include "Color.hpp"
#include "raytrace_objects/RayTracableObject.hpp"
#include "math/vectors.hpp"

namespace RayTracing {
    struct LocalRay;

    struct HitInfo {
        bool hit;
        Vec3 hitPoint;
        Vec3 normal;
        float distance;
        bool isLight = false;
    };

    struct Ray {
        Vec3 origin;
        Vec3 direction;
        std::vector<RGBf> colors;

        unsigned idX, idY;

        RGBf lightColor{0, 0, 0, 0};
        float totalDistance = 0;

        Vec3 reflectAt(const Vec3 &location, const Vec3 &normal, float totalReflection = 0.9);

        [[nodiscard]] LocalRay toLocalRay(const Transform &transform) const;

        [[nodiscard]] HitInfo intersectSphere(const Vec3 &sphereCenter, float sphereRadius) const;
    };

    struct LocalRay : public Ray {
        /**
         * Calculate hit information of triangle
         * @param triangle points of the triangle
         * @param customNormal
         * @return intersection point on triangle
         */
        [[nodiscard]] HitInfo intersectTriangle(Vec3 triangle[], const Vec3 &customNormal) const;

        bool intersectsBoundingBox(const BoundingBox &box);
    };
}