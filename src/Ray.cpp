#include "Ray.hpp"

#include "vectors.hpp"

namespace RayTracing {
    Ray::HitInfo Ray::intersectTriangle(Vec3 triangle[3], Vec3 customNormal) const {
        constexpr float epsilon = std::numeric_limits<float>::epsilon();

        Vec3 edgeAB = triangle[1] - triangle[0];
        Vec3 edgeAC = triangle[2] - triangle[0];
        Vec3 normal = Vec3::cross(edgeAB, edgeAC);
        Vec3 ao = origin - triangle[0];
        Vec3 dao = Vec3::cross(ao, direction);

        float det = -Vec3::dot(direction, normal);
        float invDet = 1.0f / det;

        float dst = Vec3::dot(ao, normal) * invDet;
        float u = Vec3::dot(edgeAC, dao) * invDet;
        float v = -Vec3::dot(edgeAB, dao) * invDet;
        float w = 1 - u - v;

        return {
            .hit = det > epsilon && dst >= 0 && u >= 0 && v >= 0 && w >= 0,
            .hitPoint = origin + (direction * dst),
            .normal = customNormal, // triangle.normA * w + triangle.normB * u + triangle.normC * v
            .dst = dst
        };
    }

    Ray::HitInfo Ray::intersectSphere(Vec3 sphereCenter, float sphereRadius) const {
        Vec3 offsetRayOrigin = origin - sphereCenter;

        float a = Vec3::dot(direction, direction);
        float b = 2 * Vec3::dot(offsetRayOrigin, direction);
        float c = Vec3::dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

        float discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            float dst = (-b - sqrt(discriminant)) / (2 * a);

            if (dst >= 0) {
                return {
                    .hit = true,
                    .hitPoint = origin + (direction * dst),
                    .normal = (origin + (direction * dst) - sphereCenter).normalized(),
                    .dst = dst
                };
            }
        }
        return {
            .hit = false
        };
    }

    float randf() {
        return rand() / (float) RAND_MAX;
    }

    Vec3 randomHemisphereReflection() {
        return Vec3{randf(), randf(), randf()}.normalized();
    }

    float sign(float x) {
        if (x > 0) return 1;
        if (x < 0) return -1;
        return 0;
    }

    Vec3 Ray::reflectAt(const Vec3& location, const Vec3 &normal, float totalReflection) {
        this->origin = location;
        float dot = Vec3::dot(direction, normal);
        Vec3 totalReflectionVec = direction - (normal * dot) * 2;
        Vec3 rhf = randomHemisphereReflection();
        rhf = rhf * sign(Vec3::dot(normal, rhf));
        //this->direction = (totalReflectionVec * totalReflection + rhf * (1-totalReflection)).normalized();
        this->direction = totalReflectionVec;
        return direction;
    }

}
