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
    Bitmap bitmap;
    bitmap.width = width;
    bitmap.height = height;
    bitmap.pixels = (Color *)malloc(width * height * sizeof(Color));

    if (bitmap.pixels) {
        // Initialize all pixels to black
        for (int i = 0; i < width * height; i++) {
            bitmap.pixels[i].r = 0;
            bitmap.pixels[i].g = 0;
            bitmap.pixels[i].b = 0;
        }
    }

    return bitmap;
}

/**
 * @brief Loads a bitmap (.bmp) file and stores the pixel data in an RGB array.
 *
 * @param filename Path to the bitmap file to load.
 * @param bitmap Pointer to a Bitmap struct where the loaded image data will be stored.
 * @return int Returns 1 on success, 0 on failure.
 */
int Bitmap_load(const char *filename, Bitmap bitmap) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }

    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    if (fileHeader.bfType != 0x4D42) { // 'BM' in hexadecimal
        fclose(file);
        fprintf(stderr, "Not a valid BMP file.\n");
        return 0;
    }

    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    if (infoHeader.biBitCount != 24) { // Only support 24-bit BMP
        fclose(file);
        fprintf(stderr, "Only 24-bit BMP files are supported.\n");
        return 0;
    }

    bitmap.width = infoHeader.biWidth;
    bitmap.height = abs(infoHeader.biHeight);
    bitmap.pixels = (Color *)malloc(bitmap.width * bitmap.height * sizeof(Color));

    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    int row_padded = (bitmap.width * 3 + 3) & (~3);
    uint8_t *row = (uint8_t *)malloc(row_padded);

    for (int y = 0; y < bitmap.height; y++) {
        fread(row, sizeof(uint8_t), row_padded, file);
        for (int x = 0; x < bitmap.width; x++) {
            int pixel_index = (bitmap.height - 1 - y) * bitmap.width + x;
            bitmap.pixels[pixel_index].b = row[x * 3 + 0];
            bitmap.pixels[pixel_index].g = row[x * 3 + 1];
            bitmap.pixels[pixel_index].r = row[x * 3 + 2];
        }
    }

    free(row);
    fclose(file);
    return 1;
}

/**
 * @brief Saves an RGB array as a bitmap (.bmp) file.
 *
 * @param filename Path to save the bitmap file.
 * @param bitmap Pointer to the Bitmap struct containing image data to save.
 * @return int Returns 1 on success, 0 on failure.
 */
int Bitmap_save(const char *filename, const Bitmap bitmap) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return 0;
    }

    const int row_padded = (bitmap.width * 3 + 3) & (~3);
    const int padding_size = row_padded - bitmap.width * 3;

    BITMAPFILEHEADER fileHeader = {0};
    fileHeader.bfType = 0x4D42;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfSize = fileHeader.bfOffBits + row_padded * bitmap.height;

    BITMAPINFOHEADER infoHeader = {0};
    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = bitmap.width;
    infoHeader.biHeight = bitmap.height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biSizeImage = row_padded * bitmap.height;

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    uint8_t *padding = (uint8_t *)calloc(padding_size, 1);

    for (int y = 0; y < bitmap.height; y++) {
        for (int x = 0; x < bitmap.width; x++) {
            int pixel_index = (bitmap.height - 1 - y) * bitmap.width + x;
            fwrite(&bitmap.pixels[pixel_index].b, 1, 1, file);
            fwrite(&bitmap.pixels[pixel_index].g, 1, 1, file);
            fwrite(&bitmap.pixels[pixel_index].r, 1, 1, file);
        }
        fwrite(padding, 1, padding_size, file);
    }

    free(padding);
    fclose(file);
    return 1;
}

/**
 * @brief Frees the memory allocated for the pixel data in the Bitmap struct.
 *
 * @param bitmap Pointer to the Bitmap struct to free.
 */
void Bitmap_free(Bitmap bitmap) {
    free(bitmap.pixels);
    bitmap.pixels = NULL;
}

/**
 * @brief Sets the color of a pixel in the bitmap at the specified coordinates.
 *
 * @param bitmap Pointer to the Bitmap struct where the pixel will be set.
 * @param x The x-coordinate of the pixel (0 to width-1).
 * @param y The y-coordinate of the pixel (0 to height-1).
 * @param color The color to set the pixel to.
 */
void Bitmap_put_pixel(Bitmap* bitmap, const int x, const int y, const Color color) {
    // Check for valid coordinates
    if (x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height) {
        return;
    }

    // Calculate the index in the pixel array
    int pixel_index = (bitmap->height - 1 - y) * bitmap->width + x; // Invert y for correct BMP format

    // Set the pixel color
    bitmap->pixels[pixel_index] = color;
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
void Bitmap_draw_line(Bitmap* bitmap, const int x0, const int y0, const int x1, const int y1, const Color color) {
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
void Bitmap_antialias_bitmap(const Bitmap *bitmap) {
    const int width = bitmap->width;
    const int height = bitmap->height;

    // Create a copy of the original pixels to avoid overwriting during computation
    Color *original_pixels = malloc(width * height * sizeof(Color));
    if (!original_pixels) return;
    for (int i = 0; i < width * height; i++) {
        original_pixels[i] = bitmap->pixels[i];
    }

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
                        const Color neighbor_pixel = original_pixels[ny * width + nx];
                        red_total += neighbor_pixel.r;
                        green_total += neighbor_pixel.g;
                        blue_total += neighbor_pixel.b;
                        sample_count++;
                    }
                }
            }

            // Compute the average color and set it to the current pixel
            Color averaged_color;
            averaged_color.r = red_total / sample_count;
            averaged_color.g = green_total / sample_count;
            averaged_color.b = blue_total / sample_count;
            bitmap->pixels[y_width + x] = averaged_color;
        }
    }

    // Free the temporary copy of the original pixels
    free(original_pixels);
}