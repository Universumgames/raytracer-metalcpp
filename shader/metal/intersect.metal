#include <metal_stdlib>
#include "shader_types.hpp"

using namespace metal;

#define epsilon 1.192093E-07

Metal_Intersection intersectSphere(Metal_LocalRay ray, simd::float3 sphereCenter, float sphereRadius){
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

Metal_Intersection intersectTriangle(Metal_LocalRay ray, float3 triangle[3], float3 customNormal) {
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

bool intersectsBoundingBox(Metal_LocalRay ray, Metal_BoundingBox box) {
    float tmin = -INFINITY;
    float tmax = INFINITY;

    for (int i = 0; i < 3; i++) {
        if (abs(ray.direction[i]) < epsilon) {
            if (ray.origin[i] < box.min[i] || ray.origin[i] > box.max[i]) {
                return false;
            }
        } else {
            float invD = 1.0f / ray.direction[i];
            float t0 = (box.min[i] - ray.origin[i]) * invD;
            float t1 = (box.max[i] - ray.origin[i]) * invD;
            if (invD < 0.0f) {
                float temp = t0;
                t0 = t1;
                t1 = temp;
            }
            tmin = max(tmin, t0);
            tmax = min(tmax, t1);
            if (tmax <= tmin) {
                return false;
            }
        }
    }
    return tmax >= tmin && tmin >= 0;
}

Metal_LocalRay toLocalRay(Metal_Ray ray, simd::float4x4 inverseTransform, simd::float4x4 inverseRotate) {
    float4 origin4 = inverseTransform * float4(ray.origin, 1);
    float4 direction4 = inverseRotate * float4(ray.direction, 0);
    return {
        .origin = float3(origin4.x, origin4.y, origin4.z),
        .direction = float3(direction4.x, direction4.y, direction4.z)
    };
}

float pseudoRandom01(float seed){
    return fract(sin(seed) * 43758.5453123);
}

float3 randomHemisphereReflection(float3 normal){
    float3 randVec = float3(pseudoRandom01(normal.x + normal.y), pseudoRandom01(normal.y + normal.z), pseudoRandom01(normal.z + normal.x));
    float3 result = normalize(randVec + normal);
    if(dot(result, normal) < 0){
        result = -result;
    }
    return result;
}

Metal_Ray reflectAt(Metal_Ray ray, simd::float3 point, simd::float3 normal, float totalReflection){
    float dotProduct = dot(ray.direction, normal);
    float3 totalReflectionVec = ray.direction -  (2.0f * dotProduct);
    float3 rhf = randomHemisphereReflection(normal);
    return {
        .origin = point,
        .direction = normalize(totalReflectionVec * totalReflection + rhf * (1 - totalReflection))
    };
}