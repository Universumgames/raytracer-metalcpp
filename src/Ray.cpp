#include "Ray.hpp"

#include <random>

#include "math/vectors.hpp"

namespace RayTracing {
    HitInfo LocalRay::intersectTriangle(Vec3 triangle[3], const Vec3 &customNormal) const {
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
            .hit = det > epsilon && dst >= epsilon && u >= 0 && v >= 0 && w >= 0,
            .hitPoint = origin + (direction * dst),
            .normal = customNormal, // triangle.normA * w + triangle.normB * u + triangle.normC * v
            .distance = dst
        };
    }

    HitInfo Ray::intersectSphere(const Vec3 &sphereCenter, float sphereRadius) const {
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
                    .distance = dst
                };
            }
        }
        return {
            .hit = false
        };
    }

    bool LocalRay::intersectsBoundingBox(const BoundingBox &box) {
        float tmin = -INFINITY;
        float tmax = INFINITY;

        if (box.contains(origin)) return true;

        for (auto axis: {Vec3::Direction::X_AXIS, Vec3::Direction::Y_AXIS, Vec3::Direction::Z_AXIS}) {
            if (direction[axis] != 0) {
                float t1 = (box.minPos[axis] - origin[axis]) / direction[axis];
                float t2 = (box.maxPos[axis] - origin[axis]) / direction[axis];
                if (t1 > t2)
                    std::swap(t1, t2);
                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);
            } else {
                if (origin[axis] < box.minPos[axis] || origin[axis] > box.maxPos[axis]) {
                    return false;
                }
            }
        }
        return tmax >= tmin && tmin >= 0;
    }

    // native cpp implementation
    /*Vec3 randomHemisphereReflection(const Vec3 &normal) {
        static float sigma = 0.8f;
        static thread_local std::mt19937 gen(std::random_device{}());
        std::normal_distribution<float> dist(0.0f, sigma);
        Vec3 randVec{dist(gen), dist(gen), dist(gen)};
        Vec3 result = (normal + randVec).normalized();

        // Ensure the new vector is in the same hemisphere as dir
        if (result.dot(normal) < 0)
            result = result * -1.0f;
        return result;
    }*/

    /// get fractional part of number
    float frac(float f) {
        return f - (long) f;
    }

    /// pseudo random number generation
    float pseudoRandom(float seed) {
        return frac(sin(seed * 4123.2345) * 43758.5453123) * 345.75467 * 9435.834123546;
    }

    Vec3 randomHemisphereReflection(Vec3 normal) {
        Vec3 randVec = Vec3(pseudoRandom(normal.x() * 598.1256 + normal.y() * 414.5788),
                            pseudoRandom(normal.y() * 358.3404 + normal.z() * 692.6398),
                            pseudoRandom(normal.z() * 928.3458 + normal.x() * 348.34575)).normalized();
        if (Vec3::dot(randVec, normal) < 0) {
            randVec = randVec * -1;
        }
        return randVec;
    }

    Vec3 Ray::reflectAt(const Vec3 &location, const Vec3 &normal, float totalReflection) {
        this->origin = location;
        float dot = Vec3::dot(direction, normal);
        Vec3 totalReflectionVec = direction - (normal * dot) * 2;
        Vec3 rhf = randomHemisphereReflection(normal);
        this->direction = (totalReflectionVec * totalReflection + rhf * (1 - totalReflection)).normalized();
        //this->direction = totalReflectionVec;
        return direction;
    }

    LocalRay Ray::toLocalRay(const Transform &transform) const {
        return LocalRay{
            transform.getInverseTransformedPosition(origin), transform.getTransformedRayDirection(direction)
        };
    }
}
