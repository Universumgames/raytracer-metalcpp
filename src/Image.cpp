#include "Image.hpp"

namespace RayTracing {
    Image::Image(unsigned int width, unsigned int height) {
        this->width = width;
        this->height = height;

        this->image = (RGBA8 **) malloc(sizeof(RGBA8 *) * height);
        for (int i = 0; i < height; i++) {
            this->image[i] = (RGBA8 *) malloc(sizeof(RGBA8) * width);
        }
    }

    RGBA8 Image::getPixel(unsigned int x, unsigned int y) {
        return image[y][x];
    }

    void Image::setPixel(unsigned int x, unsigned int y, RGBA8 color) {
        image[height - y - 1][x] = color;
        // invert y axis to convert from (0,0) as bottom left (from camera) to as top left TODO find better solution
    }

    void Image::setPixel(unsigned int x, unsigned int y, RGBf color) {
        image[height - y - 1][x] = color.toRGBA8();
        // invert y axis to convert from (0,0) as bottom left (from camera) to as top left TODO find better solution
    }

    RGBA8 *Image::valueAt(unsigned int x, unsigned int y) {
        return &image[height - y - 1][x];
    }

    RGBA8 &Image::operator[](unsigned int x, unsigned int y) {
        return image[height - y - 1][x];
    }
}
