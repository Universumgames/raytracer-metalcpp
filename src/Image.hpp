#pragma once
#include "Color.hpp"

namespace RayTracing {
    class Image {
    private:
        unsigned int width, height;

        Color **image;

    public:
        Image() = delete;

        Image(unsigned int width, unsigned int height);

        void setPixel(unsigned int x, unsigned int y, Color color);

        Color getPixel(unsigned int x, unsigned int y);

        Color *valueAt(unsigned int x, unsigned int y);
    };
}
