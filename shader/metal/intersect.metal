#include <metal_stdlib>
#include "shader_types.hpp"

using namespace metal;

#define epsilon 1.192093E-07

Metal_Intersection intersectSphere(Metal_Ray ray, simd::float3 sphereCenter, float sphereRadius){
    float3 offsetRayOrigin = ray.origin - sphereCenter;

    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(offsetRayOrigin, ray.direction);
    float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        float dst = (-b - sqrt(discriminant)) / (2 * a);

        if (dst >= 0) {
            return {
                .hit = true,
                .hitPoint = ray.origin + (ray.direction * dst),
                .normal = normalize(ray.origin + (ray.direction * dst) - sphereCenter),
                .distance = dst
            };
        }
    }
    return {.hit = false };
}

Metal_Intersection intersectTriangle(Metal_Ray ray, float3 triangle[3], float3 customNormal) {
    float3 edgeAB = triangle[1] - triangle[0];
    float3 edgeAC = triangle[2] - triangle[0];
    float3 normal = cross(edgeAB, edgeAC);
    float3 ao = ray.origin - triangle[0];
    float3 dao = cross(ao, ray.direction);
    
    float det = -dot(ray.direction, normal);
    float invDet = 1.0f / det;
    
    float dst = dot(ao, normal) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1 - u - v;
    
    
    return {
        .hit = det > epsilon && dst >= 0 && u >= 0 && v >= 0 && w >= 0,
        .hitPoint = ray.origin + (ray.direction * dst),
        .normal = customNormal, // triangle.normA * w + triangle.normB * u + triangle.normC * v
        .distance = dst
    };
}
