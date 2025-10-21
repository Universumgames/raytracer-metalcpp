#pragma once
#include "Color.hpp"
#include "vectors.hpp"

namespace RayTracing {
    struct Ray {
        struct HitInfo {
            bool hit;
            Vec3 hitPoint;
            Vec3 normal;
            float dst;
        };

        Vec3 origin;
        Vec3 direction;
        std::vector<Vec4> colors;
        float dst = 0;

        unsigned idX, idY;

        /**
         * Calculate hit information of triangle
         * @param triangle points of the triangle
         * @return intersection point on triangle
         */
        [[nodiscard]] HitInfo intersectTriangle(Vec3 triangle[3], Vec3 normal) const;

        [[nodiscard]] HitInfo intersectSphere(Vec3 sphereCenter, float sphereRadius) const;

        Vec3 reflectAt(const Vec3& location, const Vec3 &normal, float totalReflection = 1);


    };
}
