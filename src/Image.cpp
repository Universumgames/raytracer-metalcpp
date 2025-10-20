
#include "Image.hpp"

namespace RayTracing {
    Image::Image(unsigned int width, unsigned int height) {
        this->width = width;
        this->height = height;

        this->image = (Color **) malloc(sizeof(Color *) * height);
        for (int i = 0; i < height; i++) {
            this->image[i] = (Color *) malloc(sizeof(Color) * width);
        }
    }

    Color Image::getPixel(unsigned int x, unsigned int y) {
        return image[y][x];
    }

    void Image::setPixel(unsigned int x, unsigned int y, Color color) {
        image[y][x] = color;
    }

    Color *Image::valueAt(unsigned int x, unsigned int y) {
        return &image[y][x];
    }
}