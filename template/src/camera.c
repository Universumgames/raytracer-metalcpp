#include "../include/camera.h"
#include <math.h>

/**
 * @brief Initializes a new camera with the specified position and look-at target.
 *
 * The camera's forward direction is computed as the normalized vector from the position to the target.
 * The right vector is calculated as the cross product of forward and up vectors,
 * and then normalized. The up vector is recalculated to ensure it is perpendicular
 * to both forward and right vectors.
 *
 * @param position The position of the camera in 3D space.
 * @param target The point in 3D space that the camera is looking at.
 * @param up The initial up direction for the camera.
 * @return Camera The initialized camera.
 */
Camera Camera_new(const Vector3D position, const Vector3D target, const Vector3D up) {
    Camera camera;
    camera.position = position;

    // Calculate forward vector (normalized direction from position to target)
    camera.forward = Vector3D_normalize(Vector3D_subtract(target, position));

    // Calculate right vector as the cross product of forward and up, then normalize it
    camera.right = Vector3D_normalize(Vector3D_cross(camera.forward, up));

    // Recalculate up vector to ensure it is perpendicular to forward and right
    camera.up = Vector3D_cross(camera.right, camera.forward);

    return camera;
}

/**
 * @brief Computes the direction from the camera to a pixel on the screen.
 *
 * The function calculates the direction by adjusting the forward vector of the camera
 * based on the field of view and the relative position of the pixel on the screen.
 *
 * @param camera The camera from which the ray originates.
 * @param x The x-coordinate of the pixel on the screen.
 * @param y The y-coordinate of the pixel on the screen.
 * @param screen_width The width of the screen in pixels.
 * @param screen_height The height of the screen in pixels.
 * @param fov The field of view in degrees.
 * @return Vector3D The direction vector from the camera to the pixel.
 */
Vector3D Camera_get_ray_direction(const Camera *camera, const double x, const double y, const int screen_width, const int screen_height, const double fov) {
    // Convert field of view from degrees to radians
    const double aspect_ratio = (double)screen_width / (double)screen_height;
    const double fov_adjustment = tan((fov * M_PI / 180.0) / 2.0);

    // Normalize screen coordinates to [-1, 1] and adjust for aspect ratio and FOV
    const double pixel_ndc_x = ((x + 0.5) / screen_width) * 2.0 - 1.0;
    const double pixel_ndc_y = 1.0 - ((y + 0.5) / screen_height) * 2.0;

    const Vector3D ray_direction = Vector3D_normalize(
        Vector3D_add(
            Vector3D_add(camera->forward, Vector3D_scale(camera->right, pixel_ndc_x * aspect_ratio * fov_adjustment)), Vector3D_scale(camera->up, pixel_ndc_y * fov_adjustment)
        )
    );

    return ray_direction;
}
