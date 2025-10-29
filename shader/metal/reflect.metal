#include <metal_stdlib>
#include "shader_types.hpp"

using namespace metal;

float pseudoRandom01(float seed){
    return fract(sin(seed * 4123.2345) * 43758.5453123) * 345.75467 * 9435.834123546;
}

float3 randomHemisphereReflection(float3 normal){
    float3 randVec = normalize(float3(pseudoRandom01(normal.x * 598.1256 + normal.y * 414.5788), pseudoRandom01(normal.y * 358.3404 + normal.z * 692.6398), pseudoRandom01(normal.z * 928.3458 + normal.x * 348.34575)));
    if(dot(randVec, normal) < 0){
        randVec = -randVec;
    }
    return randVec;
}

Metal_Ray reflectAt(Metal_Ray ray, simd::float3 point, simd::float3 normal, float totalReflection){
    float dotProduct = dot(ray.direction, normal);
    float3 totalReflectionVec = ray.direction -  (normal * dotProduct) * 2.0f;
    float3 rhf = randomHemisphereReflection(normal);
    return {
        .origin = point - ray.direction * 0.01f,
        .direction = normalize(totalReflectionVec * totalReflection + rhf * (1 - totalReflection))
    };
}

float3 rotateNormal(float4x4 rotation, float3 normal){
    simd::float4 normal4 = rotation * float4(normal, 1);
    return normalize(float3(normal4.x, normal4.y, normal4.z));
}