#include <metal_stdlib>
#include "shader_types.hpp"

using namespace metal;

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