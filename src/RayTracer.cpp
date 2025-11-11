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
        const float aspect_ratio = (float) windowSize.getX() / (float) windowSize.getY();
        const float fov_adjustment = tan((camera->fov * M_PI / 180.0f) / 2.0f);

        Vec3 screenOrigin = Vec3::zero();
        Vec3 screen00 = screenOrigin + Vec3(-(float) windowSize.getX() / 2.0f, 0, -(float) windowSize.getY() / 2.0f);

        Vec3 camForward = Vec3::forward();
        Vec3 camRight = Vec3::right();
        Vec3 camUp = Vec3::up();

        Vec2 viewBoxScaling = getViewBoxScaling();
        Vec3 viewBoxScaling3D = {viewBoxScaling.getX(), 1, viewBoxScaling.getY()};

        std::vector<Ray> rays(getRayCount());
#ifdef DEBUG_INITIAL_RAY_GENERATION
        std::ofstream raysFile("../python/rays.py");
        std::ofstream pixelFile("../python/pixels.py");
        raysFile << "import numpy" << std::endl << "vectors = numpy.array([" << std::endl;
        pixelFile << "import numpy" << std::endl << "pixels = numpy.array([" << std::endl;
#endif

        std::vector<Vec2> offsets = getSamplingOffsets();

        for (auto y = 0; y < windowSize.getY(); y++) {
            for (auto x = 0; x < windowSize.getX(); x++) {
                for (auto s = 0; s < samplesPerPixel; s++) {
                    const Vec2 &offset = offsets[s];

                    Vec3 samplingPixelLocation = {x + offset.getX(), 0, y + offset.getY()};
                    Vec3 pixel = (screen00 + samplingPixelLocation) * viewBoxScaling3D;
                    Vec3 rayDir = (camForward +
                                   camRight * (pixel.getX() * aspect_ratio * fov_adjustment) +
                                   camUp * (pixel.getZ() * fov_adjustment)
                    ).normalized();

                    /// for dynamic vector allocation
                    // Ray ray = Ray(pixel, rayDir, Vec3::random(), {}, x, y);
                    // rays.push_back(ray);
                    /// for preallocated vector
                    unsigned index = (y * windowSize.getX() + x) * samplesPerPixel + s;
                    rays[index].origin = Vec3{};
                    rays[index].direction = rayDir;
                    rays[index].rngSeed = Vec3::random();
                    rays[index].idX = x;
                    rays[index].idY = y;


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
