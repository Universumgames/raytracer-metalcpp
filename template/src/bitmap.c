#include "../include/bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1) // Ensure the headers are packed

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

/**
 * @brief Creates a new bitmap with the specified width and height, initializing all pixels to black.
 *
 * @param width The width of the bitmap in pixels.
 * @param height The height of the bitmap in pixels.
 * @return Bitmap The newly created bitmap with all pixels initialized to black.
 */
Bitmap Bitmap_create(const int width, const int height) {
    return sfImage_create(width, height);
}


/**
 * @brief Saves an RGB array as a bitmap (.bmp) file.
 *
 * @param filename Path to save the bitmap file.
 * @param bitmap Pointer to the Bitmap struct containing image data to save.
 * @return int Returns 1 on success, 0 on failure.
 */
int Bitmap_save(const char *filename, const Bitmap bitmap) {
    return sfImage_saveToFile(bitmap, filename);
}

/**
 * @brief Frees the memory allocated for the pixel data in the Bitmap struct.
 *
 * @param bitmap Pointer to the Bitmap struct to free.
 */
void Bitmap_free(Bitmap bitmap) {
    sfImage_destroy(bitmap);
}

/**
 * @brief Sets the color of a pixel in the bitmap at the specified coordinates.
 *
 * @param bitmap Pointer to the Bitmap struct where the pixel will be set.
 * @param x The x-coordinate of the pixel (0 to width-1).
 * @param y The y-coordinate of the pixel (0 to height-1).
 * @param color The color to set the pixel to.
 */
void Bitmap_put_pixel(Bitmap bitmap, const int x, const int y, const sfColor color) {
    sfImage_setPixel(bitmap, x, y, color);
}

/**
 * @brief Draws a line on the bitmap from (x0, y0) to (x1, y1) with the specified color.
 *
 * Uses Bresenham's line algorithm to plot the line.
 * @see https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *
 * @param bitmap Pointer to the Bitmap struct where the line will be drawn.
 * @param x0 The starting x-coordinate of the line.
 * @param y0 The starting y-coordinate of the line.
 * @param x1 The ending x-coordinate of the line.
 * @param y1 The ending y-coordinate of the line.
 * @param color The color to draw the line with.
 */
void Bitmap_draw_line(Bitmap bitmap, const int x0, const int y0, const int x1, const int y1, const sfColor color) {
    int x = x0;
    int y = y0;
    const int dx = abs(x1 - x0);
    const int dy = abs(y1 - y0);
    const int sx = (x0 < x1) ? 1 : -1;
    const int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        // Set the pixel at the current position
        Bitmap_put_pixel(bitmap, x, y, color);

        // Check if we've reached the end point
        if (x == x1 && y == y1) break;

        // Calculate the error term and adjust coordinates
        const int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

/**
 * @brief Applies a simple box blur for antialiasing to the given bitmap.
 *
 * This function takes the average color of each pixel and its neighboring pixels (3x3 grid),
 * and replaces the original pixel color with the average. This reduces jagged edges and
 * smooths out the image.
 * @see https://en.wikipedia.org/wiki/Supersampling
 *
 * @param bitmap Pointer to the Bitmap struct to apply antialiasing to.
 */
void Bitmap_antialias_bitmap(const Bitmap bitmap) {
    const unsigned int width = sfImage_getSize(bitmap).x;
    const unsigned int height = sfImage_getSize(bitmap).y;

    // Create a copy of the original pixels to avoid overwriting during computation
    sfImage* original_pixels = sfImage_copy(bitmap);

    // Loop over each pixel in the bitmap
    for (int y = 0; y < height; y++) {
        const int y_width = y * width;
        for (int x = 0; x < width; x++) {
            int red_total = 0, green_total = 0, blue_total = 0;
            int sample_count = 0;

            // Loop over the 3x3 grid centered on the current pixel
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    const int nx = x + dx;
                    const int ny = y + dy;

                    // Ensure the neighboring pixel is within bounds
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        // const Color neighbor_pixel = original_pixels[ny * width + nx];
                        sfColor neighbor_pixel = sfImage_getPixel(bitmap, nx, ny);
                        red_total += neighbor_pixel.r;
                        green_total += neighbor_pixel.g;
                        blue_total += neighbor_pixel.b;
                        sample_count++;
                    }
                }
            }

            // Compute the average color and set it to the current pixel
            sfColor averaged_color;
            averaged_color.r = red_total / sample_count;
            averaged_color.g = green_total / sample_count;
            averaged_color.b = blue_total / sample_count;
            averaged_color.a = 255;
            // bitmap->pixels[y_width + x] = averaged_color;
            sfImage_setPixel(bitmap, x, y, averaged_color);
        }
    }

    // Free the temporary copy of the original pixels
    sfImage_destroy(original_pixels);
}