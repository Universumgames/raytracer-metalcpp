#include <metal_stdlib>

#include "shader_types.hpp"

using namespace metal;

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::raytrace() C++ function, used to perform the calculation on a GPU.
kernel void raytrace(
                     constant Metal_RayTraceSettings& settings [[ buffer(0) ]],
                     const device Metal_Ray* rays [[ buffer(1) ]],
                     device Metal_MeshRayTraceableObject* meshObjects [[ buffer(2) ]],
                     device float3* meshVertices [[ buffer(3) ]],
                     device int* meshIndices [[ buffer(4) ]],
                     device float3* meshNormals [[ buffer(5) ]],
                     device Metal_SphereRayTraceableObject* sphereObjects [[ buffer(6) ]],
                     device Metal_Light* lights [[ buffer(7) ]],
                     device float4* result [[ buffer(8) ]],
                     uint3 gid [[thread_position_in_grid]],
                     uint3 gridSize [[threads_per_grid]])
{
    uint idx = (gid.y * gridSize.x + gid.x) * gridSize.z + gid.z;
    Metal_Ray currentRay = rays[idx];
    unsigned colorCount = 0;
    for(unsigned b = 0; b < settings.bounces; b++){
        Metal_Intersection currentHit = {
            .hit = false,
            .distance = INFINITY,
        };
        float3 currentRotatedNormal = float3(0.0);
        float4 currentColor = float4(0.0);

        // Meshes
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
                Metal_Intersection intersection = intersectTriangle(localRay, triangle, meshNormals[meshObject.normalsOffset + i]);
                if(intersection.hit && intersection.distance < currentHit.distance){
                    currentHit = intersection;
                    currentRotatedNormal = rotateNormal(meshObject.rotation, intersection.normal);
                    currentColor = meshObject.color;
                }
            }
        }

        // Spheres
        for(unsigned s = 0; s < settings.sphereObjectCount; s++){
            Metal_SphereRayTraceableObject sphereObject = sphereObjects[s];
            Metal_Intersection intersection = intersectSphere(currentRay, sphereObject.center, sphereObject.radius);
            if(intersection.hit && intersection.distance < currentHit.distance){
                currentHit = intersection;
                currentRotatedNormal = intersection.normal;
                currentColor = sphereObject.color;
            }
        }

        // Lights
        for(unsigned l = 0; l < settings.lightsCount; l++){
            Metal_Light light = lights[l];
            Metal_Intersection intersection = intersectSphere(currentRay, light.center, light.radius);
            if(intersection.hit && intersection.distance < currentHit.distance){
                currentHit = intersection;
                currentHit.isLight = true;
                currentRotatedNormal = intersection.normal;
                currentColor = light.color;
            }
        }

        if(currentHit.hit){
            if(currentHit.isLight){
                currentRay.lightColor = currentColor;
                b = settings.bounces; // terminate
            }else{
                currentRay.colors[colorCount] = currentColor * lightDissipationCoefficient(currentHit.distance);
                colorCount++;
                Metal_Ray newRay = reflectAt(currentRay, currentHit.hitPoint, currentRotatedNormal, 0.5f);
                currentRay.origin = newRay.origin;
                currentRay.direction = newRay.direction;
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
        finalColor += currentRay.lightColor;
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