#ifndef BITMAP_H
#define BITMAP_H

#include "color.h"

/**
 * @brief Structure representing a bitmap image.
 */
typedef struct {
  int width;          ///< Width of the image in pixels
  int height;         ///< Height of the image in pixels
  Color *pixels;      ///< Array of RGB pixels
} Bitmap;

/**
 * @brief Creates a new bitmap with the specified width and height, initializing all pixels to black.
 *
 * @param width The width of the bitmap in pixels.
 * @param height The height of the bitmap in pixels.
 * @return Bitmap The newly created bitmap with all pixels initialized to black.
 */
Bitmap Bitmap_create(int width, int height);

/**
 * @brief Loads a bitmap (.bmp) file and stores the pixel data in an RGB array.
 *
 * @param filename Path to the bitmap file to load.
 * @param bitmap Pointer to a Bitmap struct where the loaded image data will be stored.
 * @return int Returns 1 on success, 0 on failure.
 */
int Bitmap_load(const char *filename, Bitmap bitmap);

/**
 * @brief Saves an RGB array as a bitmap (.bmp) file.
 *
 * @param filename Path to save the bitmap file.
 * @param bitmap Pointer to the Bitmap struct containing image data to save.
 * @return int Returns 1 on success, 0 on failure.
 */
int Bitmap_save(const char *filename, Bitmap bitmap);

/**
 * @brief Frees the memory allocated for the pixel data in the Bitmap struct.
 *
 * @param bitmap Pointer to the Bitmap struct to free.
 */
void Bitmap_free(Bitmap bitmap);

/**
 * @brief Sets the color of a pixel in the bitmap at the specified coordinates.
 *
 * @param bitmap Pointer to the Bitmap struct where the pixel will be set.
 * @param x The x-coordinate of the pixel (0 to width-1).
 * @param y The y-coordinate of the pixel (0 to height-1).
 * @param color The color to set the pixel to.
 */
void Bitmap_put_pixel(Bitmap* bitmap, int x, int y, Color color);

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
void Bitmap_draw_line(Bitmap* bitmap, const int x0, const int y0, const int x1, const int y1, const Color color);

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
void Bitmap_antialias_bitmap(const Bitmap *bitmap);

#endif
