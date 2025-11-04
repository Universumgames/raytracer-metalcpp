#pragma once
#include "shader_types.hpp"

/**
 * Reflect ray at origin with surface normal
 * @param ray the ray to reflect
 * @param point the relfection point
 * @param normal the normal to reflect at
 * @param totalReflection percentage of perfect reflection versus random reflection in same hemispehere of normal
 * @return reflected ray
 */
Metal_Ray reflectAt(Metal_Ray ray, simd::float3 point, simd::float3 normal, float totalReflection);

/**
 * Convert the global ray to object space
 * @param ray ray to convert
 * @param inverseTransform the reverse transform of the object
 * @param inverseRotate the inverse rotation matrix of the object
 * @param inverseScale the inverse scale matrix of the object
 * @return hte ray transformed into local space for the object
 */
Metal_LocalRay toLocalRay(Metal_Ray ray, simd::float4x4 inverseTransform, simd::float4x4 inverseRotate,
                          simd::float4x4 inverseScale);

/**
 * Check intersection with a sphere
 * @param ray the ray to check the intersection
 * @param sphereCenter the center position of the sphere
 * @param sphereRadius the radius of the sphere
 * @return the intersection of the ray and sphere
 */
Metal_Intersection intersectSphere(Metal_Ray ray, simd::float3 sphereCenter, float sphereRadius);

/**
 * Check intersection with a triangle
 * @param ray the ray to check the intersection
 * @param triangle the vertices of the triangle to check the intersection with
 * @param customNormal the normal of the triangle
 * @return the intersection of the ray and sphere
 */
Metal_Intersection intersectTriangle(Metal_LocalRay ray, simd::float3 triangle[3], simd::float3 customNormal);

/**
 TODO
 */
Metal_Intersection intersectTrianglesInBox(Metal_LocalRay ray, Metal_MeshRayTraceableObject meshObject,
                                           Metal_NestedBoundingBox boundingBox, deviceint *meshIndices,
                                           device float3*meshVertices, device float3*meshNormals,
                                           device Metal_NestedBoundingBox*boundingBoxes);

/**
 * Check if the ray intersects the bounding box
 * @param ray the ray in object space
 * @param box the bounding box to check
 * @return true if the ray intersects the bounding box, false otherwise
 */
bool intersectsBoundingBox(Metal_LocalRay ray, Metal_NestedBoundingBox box);

/**
 * Generate a pseudo-random direction vector that deviates at most 90 degrees from the provided normal
 * (the values of the normal act as a seed for the pseudorandom number generator)
 * @param normal the normal the reflection should roughly be directed to
 * @return random normalized vector that roughly points in the direction of "normal"
 */
simd::float3 randomHemisphereReflection(simd::float3 normal);

/**
 * Generate a pseudo random number
 * @param seed the seed for the pseudo random number generator
 * @return pseudo random number
 */
float pseudoRandom(float seed);

/**
 * Rotate a normal with the rotation matrix
 * (Used for the ray reflection, from object space to global space)
 * @param rotation the rotation matrix
 * @param normal the normal to rotate
 */
simd::float3 rotateNormal(simd::float4x4 rotation, simd::float3 normal);

/**
 * Light dissipation function
 * @param distance the distance the light of the object has traveled
 * @return percentage the brightness has decreased after the "distance"
 */
float lightDissipationCoefficient(float distance);