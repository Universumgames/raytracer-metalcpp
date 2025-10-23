#include <metal_stdlib>

#include "shader_types.hpp"

using namespace metal;

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::uvTest() C++ function, used to perform the calculation on a GPU.
kernel void raytrace(
                     device Metal_Ray* rays [[ buffer(0) ]],
                     device float4* result,
                     uint2 index [[thread_position_in_grid]],
                     uint2 gridSize [[threads_per_grid]])
{
    uint unfoldedIndex = index.y * gridSize.x + index.x;
    Metal_Intersection i = intersectSphere(rays[unfoldedIndex], float3(0,0,0), 1);
}
