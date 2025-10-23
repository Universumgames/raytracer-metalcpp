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

Metal_Intersection intersectSphere(Metal_Ray ray, simd::float3 sphereCenter, float sphereRadius);

Metal_Intersection intersectTriangle(Metal_Ray ray, float3 triangle[3], float3 customNormal);
