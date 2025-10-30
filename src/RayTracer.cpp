#include "RayTracer.hpp"

namespace RayTracing {
    RayTracer::RayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel) {
        this->windowSize = windowSize;
        this->bounces = bounces;
        this->samplesPerPixel = samplesPerPixel;

        assert(samplesPerPixel > 0);
        assert(bounces > 0);
        assert(windowSize.getX() > 0);
        assert(windowSize.getY() > 0);
    }

    RayTracer::~RayTracer() {
    }

    unsigned RayTracer::getSamplesPerPixel() const {
        return samplesPerPixel;
    }

    unsigned RayTracer::getBounces() const {
        return bounces;
    }

    Vec2 RayTracer::getViewBoxScaling() {
        Vec2 windowSizeF = {(float) this->windowSize.getX(), (float) this->windowSize.getY()};
        Vec2 desiredSize = {1, 1};
        return desiredSize / windowSizeF;
    }

    std::vector<Ray> RayTracer::calculateStartingRays(Camera *camera) {
        const double aspect_ratio = (double) windowSize.getX() / (double) windowSize.getY();
        const double fov_adjustment = tan((camera->fov * M_PI / 180.0) / 2.0);

        Vec3 screenOrigin = Vec3::zero();
        Vec3 screen00 = screenOrigin + Vec3(-(float) windowSize.getX() / 2.0f, 0, -(float) windowSize.getY() / 2.0f);

        Vec3 camForward = Vec3::forward();
        Vec3 camRight = Vec3::right();
        Vec3 camUp = Vec3::up();

        std::vector<Ray> rays;
#ifdef DEBUG_INITIAL_RAY_GENERATION
        std::ofstream raysFile("../python/rays.py");
        std::ofstream pixelFile("../python/pixels.py");
        raysFile << "import numpy" << std::endl << "vectors = numpy.array([" << std::endl;
        pixelFile << "import numpy" << std::endl << "pixels = numpy.array([" << std::endl;
#endif

        std::vector<Vec2> offsets = getSamplingOffsets();

        for (int y = 0; y < windowSize.getY(); y++) {
            for (int x = 0; x < windowSize.getX(); x++) {
                for (const auto &offset: offsets) {
                    Vec3 samplingPixelLocation = {x + offset.getX(), 0, y + offset.getY()};
                    Vec3 pixel = (screen00 + samplingPixelLocation) * Vec3(
                                     getViewBoxScaling().getX(), 1, getViewBoxScaling().getY());
                    Vec3 rayDir = (camForward + (camRight * pixel.getX() * aspect_ratio * fov_adjustment) + (
                                       camUp * pixel.getZ() * fov_adjustment)).normalized();
                    Ray ray = Ray(pixel, rayDir, {}, x, y);
                    rays.push_back(ray);
#ifdef DEBUG_INITIAL_RAY_GENERATION
                    if (y % 32 == 0 && x % 32 == 0 && offset == offsets[0]) {
                        pixelFile << "[" << pixel.getX() << ", " << pixel.getY() << ", " << pixel.z() << "]," <<
                                std::endl;
                        raysFile << "[" << rayDir.getX() << ", " << rayDir.getY() << ", " << rayDir.z() << "]," <<
                                std::endl;
                    }
#endif
                }
            }
        }
#ifdef DEBUG_INITIAL_RAY_GENERATION
        raysFile << "])" << std::endl;
        pixelFile << "])" << std::endl;
#endif
        return rays;
    }

    std::vector<Vec2> RayTracer::getSamplingOffsets() {
        std::vector<Vec2> offsets;

        const float rows = std::floor(std::sqrt(samplesPerPixel));
        const float cols = std::ceil((float) samplesPerPixel / (float) rows);

        float dx = 1.0f / (cols + 1);
        float dy = 1.0f / (rows + 1);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (offsets.size() >= samplesPerPixel) break;
                float x = (c + 1.0f) * dx;
                float y = (r + 1.0f) * dy;
                offsets.emplace_back(x, y);
            }
        }
        return offsets;
    }
}