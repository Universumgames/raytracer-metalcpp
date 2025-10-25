#pragma once
#include <simd/simd.h>

struct Metal_Ray {
    simd::float3 origin;
    simd::float3 direction;
    simd::float4 colors[6];
};

struct Metal_Intersection {
    simd::float3 hitPoint;
    bool hit;
    simd::float3 normal;
    float distance;
    bool isLight;
};

struct Metal_RayTraceSettings {
    unsigned bounces;
    unsigned samplesPerPixel;
    unsigned meshObjectCount;
    unsigned sphereObjectCount;
    unsigned lightsCount;
};

struct Metal_MeshRayTraceableObject {
    simd::float4x4 transform;
    simd::float4 color;
    unsigned vertIndicesOffset;
    unsigned vertIndicesCount;
    unsigned vertPositionsOffset;
};

struct Metal_SphereRayTraceableObject {
    simd::float4x4 transform;
    float radius;
    simd::float4 color;
};

struct Metal_Light {
    simd::float4x4 transform;
    float intensity;
    simd::float4 color;
    float radius;
};

Metal_Intersection intersectSphere(Metal_Ray ray, simd::float3 sphereCenter, float sphereRadius);

Metal_Intersection intersectTriangle(Metal_Ray ray, simd::float3 triangle[3], simd::float3 customNormal);
