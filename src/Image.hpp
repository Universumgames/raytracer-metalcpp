#pragma once
#include "Color.hpp"

namespace RayTracing {
    /// Simple image class for storing pixel data
    class Image {
    private:
        unsigned int width, height;

        RGBA8 **image;

    public:
        Image() = delete;

        Image(unsigned int width, unsigned int height);

        Image(const Vec2u &imageSize) : Image(imageSize.getX(), imageSize.getY()) {
        }

        /**
         * Sets the color of a pixel at (x, y)
         * @param x x coordinate
         * @param y y coordinate
         * @param color 8Bit RGBA Color to set
         */
        void setPixel(unsigned int x, unsigned int y, RGBA8 color);

        /**
         * Sets the color of a pixel at (x, y)
         * @param x x coordinate
         * @param y y coordinate
         * @param color floating point RGB Color to set
         */
        void setPixel(unsigned int x, unsigned int y, RGBf color);

        /**
         * Get a reference to the color of a pixel at (x, y)
         * @param x x coordinate
         * @param y y coordinate
         * @return Reference to the 8Bit RGBA Color at the specified pixel
         */
        RGBA8 &operator[](unsigned int x, unsigned int y);

        /**
         * Get the color of a pixel at (x, y)
         * @param x x coordinate
         * @param y y coordinate
         * @return 8Bit RGBA Color at the specified pixel
         */
        RGBA8 getPixel(unsigned int x, unsigned int y);

        /**
         * Get a pointer to the color of a pixel at (x, y)
         * @param x x coordinate
         * @param y y coordinate
         * @return Pointer to the 8Bit RGBA Color at the specified pixel
         */
        RGBA8 *valueAt(unsigned int x, unsigned int y);
    };
}