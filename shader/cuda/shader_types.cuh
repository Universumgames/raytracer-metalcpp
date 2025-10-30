#pragma once
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

__global__ void cuda_uvTest(float4* out, uint2 screenSize);

extern "C" void callUVTest(float4* devOut, dim3 threadsPerBlock, dim3 blocks, uint2 screenSize);