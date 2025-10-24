#include <metal_stdlib>
#include "shader_types.hpp"
using namespace metal;

/// This is a Metal Shading Language (MSL) function equivalent to the SequentialRayTracer::uvTest() C++ function, used to perform the calculation on a GPU.
kernel void uvTest(device float4 *out [[ buffer(0) ]],
                   constant uint2 &screenSize [[ buffer(1) ]],
                   uint2 gid [[ thread_position_in_grid ]]) {

    uint idx = gid.y * screenSize.x + gid.x;
    out[idx] = float4(gid.x / (float)screenSize.x, gid.y / (float)screenSize.y, 0.0, 1.0);
}