#include "../include/light.h"

//Default ambient light intensity and color definition
const double LIGHT_AMBIENT_INTENSITY = 0.5;
const Color LIGHT_AMBIENT_COLOR = {255,255,255};

/**
 * @brief Initializes a new light with a specified position and color.
 *
 * The color intensity components (RGB) should ideally be in the range [0, 1].
 *
 * @param position The position of the light in 3D space.
 * @param color The color/intensity of the light.
 * @return Light The initialized light.
 */
Light Light_new(const Vector3D position, const Color color) {
    const Light light = {position, color};
    return light;
}
