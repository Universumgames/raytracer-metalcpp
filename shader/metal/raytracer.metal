#include <metal_stdlib>

#include "shader_methods.hpp"

using namespace metal;

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::raytrace() C++ function, used to perform the calculation on a GPU.
kernel void raytrace(
                     constant Metal_RayTraceSettings& settings [[ buffer(0) ]],
                     const device Metal_Ray* rays [[ buffer(1) ]],
                     device Metal_MeshRayTraceableObject* meshObjects [[ buffer(2) ]],
                     device float3* meshVertices [[ buffer(3) ]],
                     device int* meshIndices [[ buffer(4) ]],
                     device float3* meshNormals [[ buffer(5) ]],
                     device Metal_NestedBoundingBox* boundingBoxes [[ buffer(6) ]],
                     device Metal_SphereRayTraceableObject* sphereObjects [[ buffer(7) ]],
                     device Metal_Light* lights [[ buffer(8) ]],
                     device float4* result [[ buffer(9) ]],
                     uint3 gid [[thread_position_in_grid]],
                     uint3 gridSize [[threads_per_grid]])
{
    uint idx = (gid.y * gridSize.x + gid.x) * gridSize.z + gid.z;
    Metal_Ray currentRay = rays[idx];
    simd::float4 colors[METAL_COLOR_COUNT_MAX];
    simd::float4 lightColor;
    unsigned colorCount = 0;
    /// bounce ray around and check for nearest intersection on each bounce
    for(unsigned b = 0; b < min(settings.bounces, METAL_COLOR_COUNT_MAX); b++){
        Metal_Intersection currentHit = {
            .hit = false,
            .distance = INFINITY,
        };
        float3 currentRotatedNormal = float3(0.0);
        float4 currentColor = float4(0.0);

        /// check intersections with meshes
        for(unsigned meshObjIndex = 0; meshObjIndex < settings.meshObjectCount; meshObjIndex++){
            Metal_MeshRayTraceableObject meshObject = meshObjects[meshObjIndex];
            Metal_LocalRay localRay = toLocalRay(currentRay, meshObject.inverseTransform, meshObject.inverseRotate, meshObject.inverseScale);
            
            /// this calculation is also done in intersectTrianglesInBox
#ifdef NAIVE_BOUNDING_BOX
            if(!intersectsBoundingBox(localRay, boundingBoxes[meshObject.boundingBoxIndex])){
                continue;
            }
            
            /// simple triangle intersection algorithm without checking for nested bounding boxes
            for(unsigned i = 0; i < meshObject.triangleCount; i++){
                int startIndicesIndex = meshObject.indicesOffset + (i * 3);
                int vertexOffset = meshObject.vertexOffset;
                float3 triangle[3] = {
                    meshVertices[vertexOffset + meshIndices[startIndicesIndex + 0]],
                    meshVertices[vertexOffset + meshIndices[startIndicesIndex + 1]],
                    meshVertices[vertexOffset + meshIndices[startIndicesIndex + 2]],
                };
                Metal_Intersection intersection = intersectTriangle(localRay, triangle, meshNormals[meshObject.normalsOffset + i]);
                if(intersection.hit && intersection.distance < currentHit.distance){
                    currentHit = intersection;
                    currentRotatedNormal = rotateNormal(meshObject.rotation, intersection.normal);
                    currentColor = meshObject.color;
                }
             }
#else
            Metal_Intersection intersection = intersectTrianglesInBox(localRay, meshObjIndex, meshObject.boundingBoxIndex, meshObjects, meshIndices, meshVertices, meshNormals, boundingBoxes);
            
            if(intersection.hit && intersection.distance < currentHit.distance){
                currentHit = intersection;
                currentRotatedNormal = rotateNormal(meshObject.rotation, intersection.normal);
                currentColor = meshObject.color;
            }
#endif
        }

        /// check intersections with spheres
        for(unsigned sphereIndex = 0; sphereIndex < settings.sphereObjectCount; sphereIndex++){
            Metal_SphereRayTraceableObject sphereObject = sphereObjects[sphereIndex];
            Metal_Intersection intersection = intersectSphere(currentRay, sphereObject.center, sphereObject.radius);
            if(intersection.hit && intersection.distance < currentHit.distance){
                currentHit = intersection;
                currentRotatedNormal = intersection.normal;
                currentColor = sphereObject.color;
            }
        }

        /// check intersections with light sources
        for(unsigned lightIndex = 0; lightIndex < settings.lightsCount; lightIndex++){
            Metal_Light light = lights[lightIndex];
            Metal_Intersection intersection = intersectSphere(currentRay, light.center, light.radius);
            if(intersection.hit && intersection.distance < currentHit.distance){
                currentHit = intersection;
                currentHit.isLight = true;
                currentRotatedNormal = intersection.normal;
                currentColor = light.color;
            }
        }

        /// update color storage and relfection when hit
        if(currentHit.hit){
            if(currentHit.isLight){
                lightColor = currentColor;
                b = settings.bounces; // terminate
            }else{
                colors[colorCount] = currentColor * lightDissipationCoefficient(currentHit.distance);
                colorCount++;
                Metal_Ray newRay = reflectAt(currentRay, currentHit.hitPoint, currentRotatedNormal, 0.5f);
                currentRay.origin = newRay.origin;
                currentRay.direction = newRay.direction;
                //currentRay.totalDistance += currentHit.distance;
            }
        }else{
            /// if the ray did not hit anything we can stop now
            b = settings.bounces; // no hit, stop bouncing
        }
    }
    float4 finalColor = float4(0.0);
    if(colorCount > 0){
        for(unsigned c = 0; c < colorCount; c++){
            finalColor += colors[c];
        }
        finalColor = finalColor / float(colorCount);
        finalColor += lightColor;
        finalColor /= 2.0f;
        finalColor.w = 1.0f;
    }else{
        finalColor = float4(0.0, 0.0, 0.0, 1.0);
    }
    result[idx] = finalColor;
}

float lightDissipationCoefficient(float distance){
    return 1.0f / (sqrt(distance / 1000.0f) + 1.0f);
}
