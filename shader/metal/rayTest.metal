#include <metal_stdlib>
#include "shader_types.hpp"

using namespace metal;

kernel void rayTest(const device Metal_Ray* rays [[ buffer(0) ]],
                   constant float3& forward [[ buffer(1) ]],
                   device float4* result,
                   uint2 gid [[thread_position_in_grid]],
                   uint2 gridSize [[threads_per_grid]])
{
    uint idx = gid.y * gridSize.x + gid.x;
    // the for-loop is replaced with a collection of threads, each of which calls this function
    float dotValue = dot(rays[idx].direction, forward);
    dotValue = dotValue * dotValue * dotValue * dotValue;
    result[idx] = float4(dotValue, dotValue, dotValue,1);
}
