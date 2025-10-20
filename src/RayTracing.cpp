#include "RayTracing.hpp"

namespace RayTracing {
    RayTracer::RayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel) {
        this->width = width;
        this->height = height;
        this->bounces = bounces;
        this->samplesPerPixel = samplesPerPixel;
        updateViewBoxScaling();
    }

    RayTracer::~RayTracer() {
    }

    Vec2 RayTracer::windowSize() const {
        return {static_cast<float>(width), static_cast<float>(height)};
    }

    Vec2 RayTracer::getViewBoxScaling() {
        Vec2 windowSize = this->windowSize();
        Vec2 desiredSize = {1,1};
        return desiredSize / windowSize;
    }

    std::vector<Ray> RayTracer::calculateStartingRays() {
        float screenDistance = 20;
        Vec3 rayOrigin = Vec3::back() * screenDistance;
        Vec3 screenOrigin = Vec3::zero();
        Vec3 screen00 = screenOrigin + Vec3(-(float) width / 2.0f, -(float) height / 2.0f, 0);

        std::vector<Ray> rays;
#ifdef DEBUG_INITIAL_RAY_GENERATION
        std::ofstream raysFile("../python/rays.py");
        std::ofstream pixelFile("../python/pixels.py");
        raysFile << "import numpy" << std::endl << "vectors = numpy.array([" << std::endl;
        pixelFile << "import numpy" << std::endl << "pixels = numpy.array([" << std::endl;
#endif

        std::vector<Vec2> offsets = getSamplingOffsets();

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (const auto& offset : offsets) {
                    Vec3 sampleLocation = Vec3(Vec2(x,y) + offset, 0);
                    Vec3 pixel = (screen00 + sampleLocation) * Vec3(getViewBoxScaling(), 1);
                    Vec3 rayDir = (rayOrigin - pixel).normalized();
                    Ray ray = Ray(pixel, rayDir);
                    rays.push_back(ray);
                }
#ifdef DEBUG_INITIAL_RAY_GENERATION
                if (y % 32 == 0 && x % 32 == 0) {
                    pixelFile << "[" << pixel.x << ", " << pixel.y << ", " << pixel.z << "]," << std::endl;
                    raysFile << "[" << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << "]," << std::endl;
                }
#endif
            }
        }
#ifdef DEBUG_INITIAL_RAY_GENERATION
        raysFile << "[0,0,0]])" << std::endl;
        pixelFile << "[0,0,1]])" << std::endl;
#endif


        return rays;
    }

    std::vector<Vec2> RayTracer::getSamplingOffsets() {
        std::vector<Vec2> offsets;

        int rows = std::floor(std::sqrt(samplesPerPixel));
        int cols = std::ceil((float)samplesPerPixel / (float)rows);

        float dx = 1.0f / (cols + 1);
        float dy = 1.0f / (rows + 1);

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (offsets.size() >= samplesPerPixel) break;
                float x = (c + 1) * dx;
                float y = (r + 1) * dy;
                offsets.emplace_back(x, y);
            }
        }
        return offsets;
    }

}
