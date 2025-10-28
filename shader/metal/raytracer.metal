#include <metal_stdlib>

#include "shader_types.hpp"

using namespace metal;

#define ray rays[idx]

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::raytrace() C++ function, used to perform the calculation on a GPU.
kernel void raytrace(
                     constant Metal_RayTraceSettings& settings [[ buffer(0) ]],
                     const device Metal_Ray* rays [[ buffer(1) ]],
                     device Metal_MeshRayTraceableObject* meshObjects [[ buffer(2) ]],
                     device float3* meshVertices [[ buffer(3) ]],
                     device int* meshIndices [[ buffer(4) ]],
                     device Metal_SphereRayTraceableObject* sphereObjects [[ buffer(5) ]],
                     device Metal_Light* lights [[ buffer(6) ]],
                     device float4* result [[ buffer(7) ]],
                     uint3 gid [[thread_position_in_grid]],
                     uint3 gridSize [[threads_per_grid]])
{
    uint idx = gid.y * gridSize.x + gid.x * gridSize.z + gid.z;
    Metal_Ray currentRay = ray;
    unsigned colorCount = 0;
    for(unsigned b = 0; b < settings.bounces; b++){
        Metal_Intersection currentHit = {
            .hit = false,
            .distance = INFINITY,
        };
        float3 currentRotatedNormal = float3(0.0);
        float4 currentColor = float4(0.0);

        for(unsigned o = 0; o < settings.meshObjectCount; o++){
            Metal_MeshRayTraceableObject meshObject = meshObjects[o];
            Metal_LocalRay localRay = toLocalRay(currentRay, meshObject.inverseTransform, meshObject.inverseRotate);
            if(!intersectsBoundingBox(localRay, meshObject.boundingBox)){
                continue;
            }
            for(unsigned i = 0; i < meshObject.triangleCount; i++){
                int startIndicesIndex = meshObject.indicesOffset + (i * 3);
                int vertexOffset = meshObject.vertexOffset;
                float3 triangle[3] = {
                    meshVertices[vertexOffset + meshIndices[startIndicesIndex + 0]],
                    meshVertices[vertexOffset + meshIndices[startIndicesIndex + 1]],
                    meshVertices[vertexOffset + meshIndices[startIndicesIndex + 2]],
                };
                Metal_Intersection intersection = intersectTriangle(localRay, triangle);
                if(intersection.hit && intersection.distance < currentHit.distance){
                    currentHit = intersection;
                    currentRotatedNormal = rotateNormal(meshObject.inverseRotate, intersection.normal);
                    currentColor = meshObject.color;
                }
            }

            for(unsigned s = 0; s < settings.sphereObjectCount; s++){
                Metal_SphereRayTraceableObject sphereObject = sphereObjects[s];
                Metal_Intersection intersection = intersectSphere(currentRay, sphereObject.center, sphereObject.radius);
                if(intersection.hit && intersection.distance < currentHit.distance){
                    currentHit = intersection;
                    currentRotatedNormal = intersection.normal;
                    currentColor = sphereObject.color;
                }
            }

            // TODO lights

            if(currentHit.hit){
                currentRay.colors[colorCount] = currentColor;
                colorCount++;
                //currentRay = reflectAt(currentRay, currentHit.hitPoint, currentRotatedNormal, 1.0f);
                //currentRay.totalDistance += currentHit.distance;
            }
        }
    }
    float4 finalColor = float4(0.0);
    if(colorCount > 0){
        for(unsigned c = 0; c < colorCount; c++){
            finalColor += currentRay.colors[c];
        }
        finalColor = finalColor / float(colorCount);
        finalColor.w = 1.0f;
    }else{
        finalColor = float4(0.0, 0.0, 0.0, 1.0);
    }
    result[idx] = finalColor;
}
