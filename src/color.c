#include "../include/color.h"

// Default color definitions
const Color COLOR_BLACK       = { 0, 0, 0 };
const Color COLOR_WHITE       = { 255, 255, 255 };
const Color COLOR_RED         = { 255, 0, 0 };
const Color COLOR_DARK_RED    = { 139, 0, 0 };
const Color COLOR_GREEN       = { 0, 255, 0 };
const Color COLOR_DARK_GREEN  = { 0, 139, 0 };
const Color COLOR_BLUE        = { 0, 0, 255 };
const Color COLOR_DARK_BLUE   = { 0, 0, 139 };
const Color COLOR_CYAN        = { 0, 255, 255 };
const Color COLOR_DARK_CYAN   = { 0, 139, 139 };
const Color COLOR_MAGENTA     = { 255, 0, 255 };
const Color COLOR_DARK_MAGENTA = { 139, 0, 139 };
const Color COLOR_YELLOW      = { 255, 255, 0 };
const Color COLOR_DARK_YELLOW = { 139, 139, 0 };
const Color COLOR_GRAY        = { 128, 128, 128 };
const Color COLOR_LIGHT_GRAY  = { 211, 211, 211 };
const Color COLOR_DARK_GRAY   = { 64, 64, 64 };
const Color COLOR_ORANGE      = { 255, 165, 0 };
const Color COLOR_DARK_ORANGE = { 205, 102, 0 };
const Color COLOR_BROWN       = { 139, 69, 19 };
const Color COLOR_DARK_BROWN  = { 92, 46, 10 };
const Color COLOR_PINK        = { 255, 192, 203 };
const Color COLOR_DARK_PINK   = { 219, 112, 147 };
const Color COLOR_PURPLE      = { 128, 0, 128 };
const Color COLOR_DARK_PURPLE = { 75, 0, 130 };
const Color COLOR_TEAL        = { 0, 128, 128 };
const Color COLOR_DARK_TEAL   = { 0, 77, 77 };

/**
 * @brief Creates a new color with the specified RGB values.
 *
 * @param r The red component (0-255).
 * @param g The green component (0-255).
 * @param b The blue component (0-255).
 * @return Color The initialized color.
 */
Color Color_new(const uint8_t r, const uint8_t g, const uint8_t b) {
    const Color color = {r, g, b};
    return color;
}

/**
 * @brief Adds two colors component-wise with clamping at 255.
 *
 * Each component of the resulting color is the sum of the corresponding components
 * of the input colors, clamped at a maximum value of 255.
 *
 * @param c1 The first color.
 * @param c2 The second color.
 * @return Color The resulting color after addition.
 */
Color Color_add(const Color c1, const Color c2) {
    Color result;
    result.r = (c1.r + c2.r > 255) ? 255 : c1.r + c2.r;
    result.g = (c1.g + c2.g > 255) ? 255 : c1.g + c2.g;
    result.b = (c1.b + c2.b > 255) ? 255 : c1.b + c2.b;
    return result;
}

/**
 * @brief Multiplies a color by a scalar value with clamping at 255.
 *
 * Each component of the resulting color is scaled by the scalar value and clamped
 * to a maximum of 255.
 *
 * @param c The color to scale.
 * @param scalar The scalar multiplier.
 * @return Color The resulting color after scaling.
 */
Color Color_scale(const Color c, const double scalar) {
    Color result;
    result.r = (c.r * scalar > 255) ? 255 : (uint8_t)(c.r * scalar);
    result.g = (c.g * scalar > 255) ? 255 : (uint8_t)(c.g * scalar);
    result.b = (c.b * scalar > 255) ? 255 : (uint8_t)(c.b * scalar);
    return result;
}
