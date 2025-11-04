#pragma once
#include <simd/simd.h>

/// Metal struct definition of a ray
struct Metal_Ray {
    simd::float3 origin;
    simd::float3 direction;
};

/// Metal struct definition of a ray in local space
struct Metal_LocalRay {
    simd::float3 origin;
    simd::float3 direction;
};

/// intersection information
struct Metal_Intersection {
    simd::float3 hitPoint;
    bool hit;
    simd::float3 normal;
    float distance;
    bool isLight;
};

/// metal struct definition of a nested bounding box
struct Metal_NestedBoundingBox {
    simd::float3 min;
    simd::float3 max;
    /// offset of indices in buffer, -1 if no indices are part of this bounidng box
    int indicesOffset;
    /// offset of normals in buffer, -1 if no normals are part of this bounding box
    int normalsOffset;
    /// number of triangles in this bounding box
    unsigned triangleCount;
    /// index of left child box in bounding box buffer, -1 if no child element exists
    int childLeftIndex;
    /// index of right child box in bounding box buffer, -1 if no child element exists
    int childRightIndex;
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
    unsigned boundingBoxIndex;
    simd::float4x4 transform;
    simd::float4x4 inverseTransform;
    simd::float4x4 rotation;
    simd::float4x4 inverseRotate;
    simd::float4x4 inverseScale;
    simd::float4 color;
    unsigned indicesOffset;
    unsigned triangleCount;
    unsigned vertexOffset;
    unsigned normalsOffset;
};

struct Metal_SphereRayTraceableObject {
    Metal_NestedBoundingBox boundingBox;
    simd::float3 center;
    float radius;
    simd::float4 color;
};

struct Metal_Light {
    Metal_NestedBoundingBox boundingBox;
    simd::float3 center;
    float intensity;
    simd::float4 color;
    float radius;
};

#define METAL_COLOR_COUNT_MAX ((unsigned)6)
/// max stack size for nested bounding box traversing
#define METAL_NESTING_BB_STACK ((unsigned) 30)

/// naive bounding box checking, not using nested structure
//#define NAIVE_BOUNDING_BOX
