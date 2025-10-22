#pragma once
#include "Image.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "vectors.hpp"

namespace RayTracing {
    class RayTracer {
    protected:
        unsigned int width, height;
        unsigned bounces;
        unsigned samplesPerPixel;

        /**
         * get the view box scaling to convert from screen space to world space
         * ratio of the view boc to the screen space
         * (1/windowSize)
         */
        Vec2 getViewBoxScaling();

        /**
         * Calculate the starting rays for the raytrace
         * @return
         */
        std::vector<Ray> calculateStartingRays(Camera* camera);

        void resolveRays(Image* image, std::vector<Ray>& rays) const;

    private:
        /**
         * For multiple rays per pixel calculate coordinate offsets for samples
         * @return geometrical centered point cloud of length samplesPerPixel
         */
        std::vector<Vec2> getSamplingOffsets();

    public:
        RayTracer() = delete;

        RayTracer(unsigned width, unsigned height, unsigned bounces, unsigned samplesPerPixel);

        virtual ~RayTracer();

        virtual std::string identifier() = 0;

        /**
         * Raytrace a scene and generate image
         * @param scene scene to raytrace
         * @return raytraced image
         */
        virtual Image *raytrace(Scene scene) = 0;

        /**
         * Simple UV Space image test, used to test basic compute pipeline
         * @return uv image
         */
        virtual Image *uvTest() = 0;

        Image* rayTest(Camera* camera);

        [[nodiscard]] Vec2 windowSize() const;

        unsigned getSamplesPerPixel() const;
        unsigned getBounces() const;
    };
}
