#include <metal_stdlib>
#include "shader_methods.hpp"

using namespace metal;

float pseudoRandom(float seed) {
    return fract(sin(seed * 4123.207643345f) * 4358.545334675123f) * 365.754674686f * 5.8341456723546f;
}

float pseudoRandom(float seed1, float seed2) {
    float pr1 = pseudoRandom(seed1);
    float pr2 = pseudoRandom(seed2);
    float lSeed1 = fract(seed1) * 100.0345950234578f;
    float lSeed2 = fract(seed2) * 100.0324579203456f;
    return fract(sin(pr1 * 4653.12456934f + pr2 * 2344.3424945953f) * 5468.54804553123f) * 3445.7549678367f +
    fract(9435.83412354f + lSeed1 * 8623.4813246734f + lSeed2 * 3437.3245812357f) * 4346.324684542f;
}

float3 randomHemisphereReflection(float3 normal, float3 seedVec){
    float3 seed = seedVec * normal;
    float3 randVec = normalize(float3(pseudoRandom(seed.x * 8349.856707256f, seed.y * 4314.578234568238f),
                                      pseudoRandom(seed.y * 3958.321343564f, seed.z * 2372.639348236458f),
                                      pseudoRandom(seed.z * 2258.323545674f, seed.x * 3578.345763467015f)));
    if(dot(randVec, normal) < 0){
        randVec = -randVec;
    }
    return randVec;
}

Metal_Ray reflectAt(Metal_Ray ray, simd::float3 point, simd::float3 normal, float totalReflection){
    float dotProduct = dot(ray.direction, normal);
    float3 totalReflectionVec = ray.direction -  (normal * dotProduct) * 2.0f;
    float3 rhf = randomHemisphereReflection(normal, ray.rngSeed);
    return {
        .origin = point - ray.direction * 0.01f,
        .direction = normalize(totalReflectionVec * totalReflection + rhf * (1 - totalReflection))
    };
}

float3 rotateNormal(float4x4 rotation, float3 normal){
    simd::float4 normal4 = rotation * float4(normal, 1);
    return normalize(float3(normal4.x, normal4.y, normal4.z));
}
