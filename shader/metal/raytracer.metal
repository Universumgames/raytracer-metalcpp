#include <metal_stdlib>

#include "shader_types.hpp"

using namespace metal;

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::uvTest() C++ function, used to perform the calculation on a GPU.
kernel void raytrace(
                     device Metal_Ray* rays [[ buffer(0) ]],
                     constant Metal_RayTraceSettings& settings [[ buffer(1) ]],
                     device Metal_MeshRayTraceableObject* meshObjects [[ buffer(2) ]],
                     device float3* meshVertices [[ buffer(3) ]],
                     device float* meshIndices [[ buffer(4) ]],
                     device Metal_SphereRayTraceableObject* sphereObjects [[ buffer(5) ]],
                     device Metal_Light* lights [[ buffer(6) ]],
                     device float4* result,
                     uint2 gid [[thread_position_in_grid]],
                     uint2 gridSize [[threads_per_grid]])
{
    uint idx = gid.y * gridSize.x + gid.x;
    for(unsigned b = 0; b < settings.bounces; b++){

    }
}
