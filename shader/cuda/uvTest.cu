#include "shader_types.cuh"

__global__ void cuda_uvTest(float4* out, uint2 screenSize) {
    unsigned i = (blockIdx.x * blockDim.x) + threadIdx.x;
    unsigned j = (blockIdx.y * blockDim.y) + threadIdx.y;
    unsigned idx = blockIdx.y * screenSize.x + blockIdx.x;
    //out[idx] = float4((float)i / (float)screenSize.x, (float)j / (float)screenSize.y, 0.0, 1.0);

    out[idx] = float4(1,0,0,1);
}

extern "C" void callUVTest(float4* devOut, dim3 threadsPerBlock, dim3 blocks, uint2 screenSize) {
    cuda_uvTest<<<blocks, threadsPerBlock>>>(devOut, screenSize);
}