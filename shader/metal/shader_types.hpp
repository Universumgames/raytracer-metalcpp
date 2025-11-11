#pragma once
#include <simd/simd.h>

/// Metal struct definition of a ray
struct Metal_Ray {
    simd::float3 origin;
    simd::float3 direction;
    simd::float3 rngSeed; /// Seed for random number generation, since shaders don't have good random functions
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
    float specularIntensity;
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
    float specularIntensity;
};

struct Metal_Light {
    Metal_NestedBoundingBox boundingBox;
    simd::float3 center;
    float intensity;
    simd::float4 color;
    float radius;
};

#define METAL_COLOR_COUNT_MAX ((unsigned)10)
/// max stack size for nested bounding box traversing
#define METAL_NESTING_BB_STACK ((unsigned) 40)

/// naive bounding box checking, not using nested structure
//#define NAIVE_BOUNDING_BOX


/// std lib math definitions
#define M_E         2.71828182845904523536028747135266250   /* e              */
#define M_LOG2E     1.44269504088896340735992468100189214   /* log2(e)        */
#define M_LOG10E    0.434294481903251827651128918916605082  /* log10(e)       */
#define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
#define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */
#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)     */
#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)        */
#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)      */