#pragma once
#include <simd/simd.h>

struct Metal_Ray {
    simd::float3 origin;
    simd::float3 direction;
    simd::float4 colors[6];
    unsigned numColors;
};

struct Metal_LocalRay {
    simd::float3 origin;
    simd::float3 direction;
};

struct Metal_Intersection {
    simd::float3 hitPoint;
    bool hit;
    simd::float3 normal;
    float distance;
    bool isLight;
};

struct Metal_BoundingBox {
    simd::float3 min;
    simd::float3 max;
};

struct Metal_RayTraceSettings {
    simd::uint2 screenSize;
    unsigned bounces;
    unsigned samplesPerPixel;
    unsigned meshObjectCount;
    unsigned sphereObjectCount;
    unsigned lightsCount;
    unsigned maxColorsPerRay;
};

struct Metal_MeshRayTraceableObject {
    Metal_BoundingBox boundingBox;
    simd::float4x4 transform;
    simd::float4x4 inverseTransform;
    simd::float4x4 inverseRotate;
    simd::float4 color;
    unsigned vertIndicesOffset;
    unsigned vertIndicesCount;
    unsigned vertPositionsOffset;
};

struct Metal_SphereRayTraceableObject {
    Metal_BoundingBox boundingBox;
    simd::float4x4 transform;
    float radius;
    simd::float4 color;
};

struct Metal_Light {
    Metal_BoundingBox boundingBox;
    simd::float4x4 transform;
    float intensity;
    simd::float4 color;
    float radius;
};

Metal_Ray reflectAt(Metal_Ray ray, simd::float3 point, simd::float3 normal, float totalReflection);

Metal_LocalRay toLocalRay(Metal_Ray ray, simd::float4x4 inverseTransform, simd::float4x4 inverseRotate);

Metal_Intersection intersectSphere(Metal_LocalRay ray, simd::float3 sphereCenter, float sphereRadius);

Metal_Intersection intersectTriangle(Metal_LocalRay ray, simd::float3 triangle[3], simd::float3 customNormal);

simd::float3 randomHemisphereReflection(simd::float3 normal);

float pseudoRandom01(float seed);