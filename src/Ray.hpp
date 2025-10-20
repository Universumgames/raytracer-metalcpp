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
        std::vector<Color> colors;

        unsigned idX, idY;

        /**
         * Calculate hit information of triangle
         * @param triangle points of the triangle
         * @return intersection point on triangle
         */
        HitInfo intersectTriangle(Vec3 triangle[3]) const;

        HitInfo intersectSphere(Vec3 sphereCenter, float sphereRadius) const;


    };
}
