#include <metal_stdlib>

#include "shader_types.hpp"

using namespace metal;

#define ray rays[idx]

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::uvTest() C++ function, used to perform the calculation on a GPU.
kernel void raytrace(
                     constant Metal_RayTraceSettings& settings [[ buffer(0) ]],
                     device Metal_Ray* rays [[ buffer(1) ]],
                     device Metal_MeshRayTraceableObject* meshObjects [[ buffer(2) ]],
                     device float3* meshVertices [[ buffer(3) ]],
                     device int* meshIndices [[ buffer(4) ]],
                     device Metal_SphereRayTraceableObject* sphereObjects [[ buffer(5) ]],
                     device Metal_Light* lights [[ buffer(6) ]],
                     device float4* result,
                     uint3 gid [[thread_position_in_grid]],
                     uint3 gridSize [[threads_per_grid]])
{
    uint idx = gid.y * gridSize.x + gid.x * gridSize.x + gid.z;
    Metal_Ray currentRay = ray;
    for(unsigned b = 0; b < settings.bounces; b++){

    }
}
