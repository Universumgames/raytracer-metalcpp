#pragma once
#include "Image.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "math/vectors.hpp"

namespace RayTracing {
    class RayTracer {
    protected:
        Vec2u windowSize;
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
        std::vector<Ray> calculateStartingRays(Camera *camera);

        void resolveRays(Image *image, std::vector<Ray> &rays, ColorBlendMode mode = AVERAGE) const;

    private:
        /**
         * For multiple rays per pixel calculate coordinate offsets for samples
         * @return geometrical centered point cloud of length samplesPerPixel
         */
        std::vector<Vec2> getSamplingOffsets();

    public:
        RayTracer() = delete;

        RayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        virtual ~RayTracer();

        /// Get the identifier of the raytracer
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

        /**
         * Simple ray test to check ray generation
         * @param camera camera to generate rays from
         * @return image with ray directions encoded as colors
         */
        virtual Image *rayTest(Camera *camera) = 0;

        /// Get the window size
        Vec2u getWindowSize() { return windowSize; }

        /// Get the number of samples per pixel
        unsigned getSamplesPerPixel() const;

        /// Get the number of bounces
        unsigned getBounces() const;
    };
}