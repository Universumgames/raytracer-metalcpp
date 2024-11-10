#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

/**
 * @brief Structure representing a color in RGB format.
 *
 * Each color component (R, G, B) is an 8-bit unsigned integer (0-255).
 */
typedef struct {
    uint8_t r; ///< Red component (0-255)
    uint8_t g; ///< Green component (0-255)
    uint8_t b; ///< Blue component (0-255)
} Color;

/**
 * Default colors
 */
extern const Color COLOR_BLACK;
extern const Color COLOR_WHITE;
extern const Color COLOR_RED;
extern const Color COLOR_DARK_RED;
extern const Color COLOR_GREEN;
extern const Color COLOR_DARK_GREEN;
extern const Color COLOR_BLUE;
extern const Color COLOR_DARK_BLUE;
extern const Color COLOR_CYAN;
extern const Color COLOR_DARK_CYAN;
extern const Color COLOR_MAGENTA;
extern const Color COLOR_DARK_MAGENTA;
extern const Color COLOR_YELLOW;
extern const Color COLOR_DARK_YELLOW;
extern const Color COLOR_GRAY;
extern const Color COLOR_LIGHT_GRAY;
extern const Color COLOR_DARK_GRAY;
extern const Color COLOR_ORANGE;
extern const Color COLOR_DARK_ORANGE;
extern const Color COLOR_BROWN;
extern const Color COLOR_DARK_BROWN;
extern const Color COLOR_PINK;
extern const Color COLOR_DARK_PINK;
extern const Color COLOR_PURPLE;
extern const Color COLOR_DARK_PURPLE;
extern const Color COLOR_TEAL;
extern const Color COLOR_DARK_TEAL;

/**
 * @brief Creates a new color with the specified RGB values.
 *
 * @param r The red component (0-255).
 * @param g The green component (0-255).
 * @param b The blue component (0-255).
 * @return Color The initialized color.
 */
Color Color_new(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Adds two colors component-wise with clamping at 255.
 *
 * @param c1 The first color.
 * @param c2 The second color.
 * @return Color The resulting color after addition.
 */
Color Color_add(Color c1, Color c2);

/**
 * @brief Multiplies a color by a scalar value with clamping at 255.
 *
 * @param c The color to scale.
 * @param scalar The scalar multiplier.
 * @return Color The resulting color after scaling.
 */
Color Color_scale(Color c, double scalar);

#endif
