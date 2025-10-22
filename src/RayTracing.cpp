#include "RayTracing.hpp"

namespace RayTracing {
    RayTracer::RayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel) {
        this->width = width;
        this->height = height;
        this->bounces = bounces;
        this->samplesPerPixel = samplesPerPixel;
    }

    RayTracer::~RayTracer() {
    }

    Vec2 RayTracer::windowSize() const {
        return Vec2{static_cast<float>(width), static_cast<float>(height)};
    }

    unsigned RayTracer::getSamplesPerPixel() const {
        return samplesPerPixel;
    }

    unsigned RayTracer::getBounces() const {
        return bounces;
    }

    Vec2 RayTracer::getViewBoxScaling() {
        Vec2 windowSize = this->windowSize();
        Vec2 desiredSize = {1,1};
        return desiredSize / windowSize;
    }

    std::vector<Ray> RayTracer::calculateStartingRays(Camera* camera) {
        const double aspect_ratio = (double)width/ (double)height;
        const double fov_adjustment = tan((camera->fov * M_PI / 180.0) / 2.0);

        Vec3 screenOrigin = Vec3::zero();
        Vec3 screen00 = screenOrigin + Vec3(-(float) width / 2.0f, -(float) height / 2.0f, 0);

        Vec3 camForward = Vec3::backward();
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

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (const auto &offset: offsets) {
                    Vec3 samplingPixelLocation = Vec3(Vec2(x, y) + offset, 0);
                    Vec3 pixel = (screen00 + samplingPixelLocation) * Vec3(getViewBoxScaling(), 1);
                    Vec3 rayDir = (camForward + (camRight * pixel.getX() * aspect_ratio * fov_adjustment) + (camUp * pixel.getY() * fov_adjustment)).normalized();
                    Ray ray = Ray(pixel, rayDir, {}, x, y);
                    rays.push_back(ray);
#ifdef DEBUG_INITIAL_RAY_GENERATION
                    if (y % 32 == 0 && x % 32 == 0 && offset == offsets[0]) {
                        pixelFile << "[" << pixel.getX() << ", " << pixel.getY() << ", " << pixel.z() << "]," << std::endl;
                        raysFile << "[" << rayDir.getX() << ", " << rayDir.getY() << ", " << rayDir.z() << "]," << std::endl;
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

    void RayTracer::resolveRays(Image *image, std::vector<Ray> &rays) const {
        for (unsigned x = 0; x < width; x++) {
            for (unsigned y = 0; y < height; y++) {
                std::vector<Ray> pixelRays;
                std::vector<RGBf> pixelColors;
                std::vector<RGBf> lightColors;

                int startIndex = (y * width + x) * samplesPerPixel;
                for (int i = 0; i < samplesPerPixel; i++) {
                    auto ray = rays[startIndex + i];
                    pixelRays.push_back(ray);
                    pixelColors.insert(pixelColors.end(), ray.colors.begin(), ray.colors.end());
                    lightColors.push_back(ray.lightColor);
                }

                /*// find all rays assigned to specified pixel, when we are assuming that the order changed
                auto it = rays.begin();
                while ((it = std::find_if(it, rays.end(), [x, y](const Ray &ray) {
                    return ray.idX == x && ray.idY == y;
                })) != rays.end()) {
                    pixelRays.push_back(*it);
                    std::vector<Color> colors = (*it).colors;
                    pixelColors.insert(pixelColors.end(), colors.begin(), colors.end());
                    ++it;
                }*/

                RGBf light = RGBf::blend(lightColors);
                RGBf avg = RGBf::blend(pixelColors);// * light;
                //Color bounceColor = Color(255 / pixelColors.size(), 255 / pixelColors.size(), 0, 255);
                image->setPixel(x, y, avg.toRGBA8());
            }
        }
    }

    std::vector<Vec2> RayTracer::getSamplingOffsets() {
        std::vector<Vec2> offsets;

        int rows = std::floor(std::sqrt(samplesPerPixel));
        int cols = std::ceil((float) samplesPerPixel / (float) rows);

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

    Image *RayTracer::rayTest(Camera *camera) {
        auto *image = new Image(width, height);
        auto rays = calculateStartingRays(camera);

        for (auto &ray: rays) {
            auto dot = ray.direction.dot(Vec3::forward());
            ray.colors.push_back(RGBf(dot,dot,dot, 1));
            ray.lightColor = RGBf(1,1,1,1);
        }

        resolveRays(image, rays);

        return image;
    }

}
