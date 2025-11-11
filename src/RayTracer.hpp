#pragma once
#include "Image.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "math/vectors.hpp"

namespace RayTracing {
    class RayTracer {
    protected:
        /**
         * calulate the view box scaling to convert from screen space to world space
         * ratio of the view boc to the screen space
         * (1/windowSize)
         * @return view box scaling
         */
        Vec2 getViewBoxScaling();

        /**
         * Calculate the starting rays for the raytrace
         * @return vector of rays starting from the camera through each pixel
         */
        std::vector<Ray> calculateStartingRays(Camera *camera);

        Scene scene;

    private:
        Vec2u windowSize;
        unsigned bounces;
        unsigned samplesPerPixel;
        /**
         * For multiple rays per pixel calculate coordinate offsets for samples
         * @return geometrical centered point cloud of length samplesPerPixel
         */
        std::vector<Vec2> getSamplingOffsets();

    public:
        RayTracer() = delete;

        /**
         * Base raytracer constructor
         * @param windowSize window size to raytrace
         * @param bounces bounce count per ray
         * @param samplesPerPixel rays per pixel
         */
        RayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel);

        virtual ~RayTracer() = default;

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
        [[nodiscard]] Vec2u getWindowSize() const { return windowSize; }

        /// Get the number of samples per pixel
        [[nodiscard]] unsigned getSamplesPerPixel() const;

        /// Get the number of bounces
        [[nodiscard]] unsigned getBounces() const;

        /// Get the total number of rays to be traced
        [[nodiscard]] unsigned getRayCount() const {
            return windowSize.getX() * windowSize.getY() * samplesPerPixel;
        }
    };
}