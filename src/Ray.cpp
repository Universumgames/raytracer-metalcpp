#include "Ray.hpp"

#include <random>

#include "vectors.hpp"

namespace RayTracing {
    HitInfo LocalRay::intersectTriangle(Vec3 triangle[3], Vec3 customNormal) const {
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
            .distance = dst
        };
    }

    HitInfo LocalRay::intersectSphere(Vec3 sphereCenter, float sphereRadius) const {
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

    bool Ray::intersectsBoundingBox(BoundingBox box) {
        auto local_origin = origin + box.center();
        auto local_ray_origin = Vec3{
            local_origin.dot(box.right()),
            local_origin.dot(box.up()),
            local_origin.dot(box.forward())
        };
        auto local_ray_direction = Vec3{
            direction.dot(box.right()),
            direction.dot(box.up()),
            direction.dot(box.forward())
        };

        auto boxSize = box.size();
        auto boxSizeHalf = boxSize / 2;
        auto localBoxMin = boxSizeHalf * -1;
        auto localBoxMax = boxSizeHalf;

        float tMin = -INFINITY;
        float tMax = INFINITY;
        for (unsigned axis = 0; axis < 3; axis++) {
            if (local_ray_direction[axis] != 0) {
                auto t1 = (localBoxMin[axis] - local_ray_origin[axis]) / local_ray_direction[axis];
                auto t2 = (localBoxMax[axis] - local_ray_origin[axis]) / local_ray_direction[axis];

                if (t1 > t2) {
                    std::swap(t1, t2);
                }
                tMin = std::max(tMin, t1);
                tMax = std::min(tMax, t2);
            }else {
                if (local_origin[axis] < localBoxMin[axis] || local_ray_origin[axis] > localBoxMax[axis]) {
                    return false;
                }
            }
        }

        if (tMax >= tMin || tMax > 0)
            return true;
        return false;
    }

    float randf() {
        return rand() / (float) RAND_MAX;
    }

    float sign(float x) {
        if (x > 0) return 1;
        if (x < 0) return -1;
        return 0;
    }

    Vec3 randomHemisphereReflection(const Vec3& normal) {
        static float sigma = 0.8f;
        static thread_local std::mt19937 gen(std::random_device{}());
        std::normal_distribution<float> dist(0.0f, sigma);
        Vec3 randVec{ dist(gen), dist(gen), dist(gen) };
        Vec3 result = (normal + randVec).normalized();

        // Ensure the new vector is in the same hemisphere as dir
        if (result.dot(normal) < 0)
            result = result * -1.0f;
        return result;
    }

    Vec3 Ray::reflectAt(const Vec3& location, const Vec3 &normal, float totalReflection) {
        this->origin = location;
        float dot = Vec3::dot(direction, normal);
        Vec3 totalReflectionVec = direction - (normal * dot) * 2;
        Vec3 rhf = randomHemisphereReflection(normal);
        this->direction = (totalReflectionVec * totalReflection + rhf * (1-totalReflection)).normalized();
        //this->direction = totalReflectionVec;
        return direction;
    }

    LocalRay Ray::toLocalRay(Transform transform) {
        //auto rotMat = transform.getRotationMatrix();
        // TODO
        return LocalRay{origin, direction};
    }

}
