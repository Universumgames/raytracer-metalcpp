#pragma once
#include "Color.hpp"
#include "raytrace_objects/RayTracableObject.hpp"
#include "vectors.hpp"

namespace RayTracing {
    struct LocalRay;

    struct Ray {
        Vec3 origin;
        Vec3 direction;
        std::vector<RGBf> colors;

        unsigned idX, idY;

        RGBf lightColor{0,0,0,0};
        float totalDistance = 0;

        Vec3 reflectAt(const Vec3& location, const Vec3 &normal, float totalReflection = 1);

        bool intersectsBoundingBox(BoundingBox box);

        LocalRay toLocalRay(Transform transform);
    };

    struct HitInfo {
        bool hit;
        Vec3 hitPoint;
        Vec3 normal;
        float distance;
        bool isLight = false;
    };

    struct LocalRay: public Ray {

        /**
         * Calculate hit information of triangle
         * @param triangle points of the triangle
         * @return intersection point on triangle
         */
        [[nodiscard]] HitInfo intersectTriangle(Vec3 triangle[3], Vec3 normal) const;

        [[nodiscard]] HitInfo intersectSphere(Vec3 sphereCenter, float sphereRadius) const;
    };
}
