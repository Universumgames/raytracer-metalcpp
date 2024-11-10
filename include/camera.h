#ifndef CAMERA_H
#define CAMERA_H

#include "vector3d.h"

/**
 * @brief Structure representing a camera in 3D space.
 *
 * The camera is defined by its position, the direction it is facing (forward),
 * and its up and right vectors for orientation.
 */
typedef struct {
    Vector3D position; ///< Position of the camera in 3D space.
    Vector3D forward;  ///< Forward direction the camera is facing.
    Vector3D up;       ///< Up vector defining the camera's vertical orientation.
    Vector3D right;    ///< Right vector defining the camera's horizontal orientation.
} Camera;

/**
 * @brief Initializes a new camera with the specified position and look-at target.
 *
 * Calculates the forward, up, and right vectors based on the position and target.
 *
 * @param position The position of the camera in 3D space.
 * @param target The point in 3D space that the camera is looking at.
 * @param up The up direction for the camera (usually Vector3D_new(0, 1, 0) for a standard orientation).
 * @return Camera The initialized camera.
 */
Camera Camera_new(Vector3D position, Vector3D target, Vector3D up);

/**
 * @brief Computes the direction from the camera to a pixel on the screen.
 *
 * This function is used to generate rays for each pixel in ray tracing.
 *
 * @param camera The camera from which the ray originates.
 * @param x The x-coordinate of the pixel on the screen.
 * @param y The y-coordinate of the pixel on the screen.
 * @param screen_width The width of the screen in pixels.
 * @param screen_height The height of the screen in pixels.
 * @param fov The field of view in degrees.
 * @return Vector3D The direction vector from the camera to the pixel.
 */
Vector3D Camera_get_ray_direction(const Camera *camera, double x, double y, int screen_width, int screen_height, double fov);

#endif
