#pragma once
#include "Color.hpp"

namespace RayTracing {
    class Image {
    private:
        unsigned int width, height;

        RGBA8 **image;

    public:
        Image() = delete;

        Image(unsigned int width, unsigned int height);

        void setPixel(unsigned int x, unsigned int y, RGBA8 color);

        RGBA8 getPixel(unsigned int x, unsigned int y);

        RGBA8 *valueAt(unsigned int x, unsigned int y);
    };
}
