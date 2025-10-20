#include <metal_stdlib>
using namespace metal;

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::uvTest() C++ function, used to perform the calculation on a GPU.
kernel void uvTest(const device uint2* pixels [[ buffer(0) ]],
                   const device float2* uv [[ buffer(1) ]],
                   device float4* result,
                   uint2 index [[thread_position_in_grid]],
                   uint2 gridSize [[threads_per_grid]])
{
    // the for-loop is replaced with a collection of threads, each of which calls this function
    result[index.y * gridSize.x + index.x] = float4(uv[index.y * gridSize.x + index.x], 0, 1);
}
