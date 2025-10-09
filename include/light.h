#ifndef LIGHT_H
#define LIGHT_H

#include <SFML/Graphics/Color.h>

#include "vector3d.h"

/**
 * Default ambient light intensity and color
 */
extern const double LIGHT_AMBIENT_INTENSITY;
extern const sfColor LIGHT_AMBIENT_COLOR;

/**
 * @brief Structure representing a light source in 3D space.
 *
 * A light has a position and a color/intensity, which affects how it illuminates objects in the scene.
 */
typedef struct {
    Vector3D position; ///< Position of the light in 3D space.
    sfColor color;     ///< Color of the light.
} Light;

/**
 * @brief Initializes a new light with a specified position and color.
 *
 * @param position The position of the light in 3D space.
 * @param color The color/intensity of the light (RGB components should be in the range [0, 1]).
 * @return Light The initialized light.
 */
Light Light_new(Vector3D position, sfColor color);

#endif
