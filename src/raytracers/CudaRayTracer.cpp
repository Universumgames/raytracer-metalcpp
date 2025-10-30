#ifdef USE_SHADER_CUDA
#include "CudaRayTracer.hpp"
#include <cuda_runtime.h>
#include <nvrtc_helper.h>
#include <helper_functions.h>

namespace RayTracing {

    CudaRayTracer::CudaRayTracer(const Vec2u& screenSize, unsigned bounces, unsigned samplesPerPixel)
            : RayTracer(screenSize, bounces, samplesPerPixel) {

        this->bufferResult = (RGBf*)calloc(getWindowSize().getX() * getWindowSize().getY() * getSamplesPerPixel(), sizeof(RGBf));
        int num_gpus = 0;
        cudaGetDeviceCount(&num_gpus);
        if (num_gpus < 1) {
            printf("no CUDA capable devices were detected\n");
            throw std::runtime_error("no CUDA capable devices were detected");
        }

        int cuda_device = gpuGetMaxGflopsDeviceIdDRV();

        cudaSetDevice(cuda_device);
        cudaGetDeviceProperties(&deviceProp, cuda_device);

        std::cout << "[" << identifier() << "] Using CUDA device " << deviceProp.name << std::endl;
    }

    Image *CudaRayTracer::outputBufferToImage(unsigned samples) {
        auto windowSize = getWindowSize();
        auto *image = new Image(windowSize.getX(), windowSize.getY());
        for (unsigned y = 0; y < windowSize.getY(); y++) {
            for (unsigned x = 0; x < windowSize.getX(); x++) {
                std::vector<RGBf> colors;
                int startIndex = (y * getWindowSize().getX() + x) * samples;
                for (unsigned s = 0; s < samples; s++) {
                    float4 color = ((float4 *) bufferResult)[startIndex + s];
                    colors.push_back(RGBf::fromFloat4(color));
                }
                image->setPixel(x, y, RGBf::blend(colors));
            }
        }
        return image;
    }

    Image *CudaRayTracer::raytrace(Scene scene) {
        auto *image = new Image(getWindowSize());
        return image;
    }

    Image *CudaRayTracer::uvTest() {
        auto* screenSize = (uint2*) new Vec2u(getWindowSize());
        size_t outSize = getWindowSize().getX() * getWindowSize().getY();
        float4* devOut = nullptr;
        auto err = cudaMalloc(&devOut, sizeof(float4) * outSize);
        if (err != cudaSuccess) {
            std::cerr << "cudaMalloc failed: " << cudaGetErrorString(err) << std::endl;
            return nullptr;
        }

        dim3 threadsPerBlock(1,1, 1);
        dim3 blocks(getWindowSize().getX() / threadsPerBlock.x, getWindowSize().getY() / threadsPerBlock.y, 1);

        callUVTest(devOut, threadsPerBlock, blocks, *screenSize);
        err = cudaDeviceSynchronize();
        if (err != cudaSuccess) {
            std::cerr << "cudaDeviceSynchronize: " << cudaGetErrorString(err) << std::endl;
            return nullptr;
        }

        err = cudaMemcpy(bufferResult, devOut, sizeof(float4) * outSize, cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            std::cerr << "cudaMemcpy failed: " << cudaGetErrorString(err) << std::endl;
        }

        delete screenSize;

        auto image = outputBufferToImage(1);
        cudaFree(devOut);

        return image;
    }

    Image *CudaRayTracer::rayTest(Camera *camera) {
        auto *image = new Image(getWindowSize());
        return image;
    }
}
#endif