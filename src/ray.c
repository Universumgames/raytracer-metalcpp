#include "../include/ray.h"

/**
 * @brief Initializes a new ray with a specified origin and direction.
 *
 * The direction vector is normalized to ensure it has unit length,
 * which is crucial for consistent calculations in ray tracing.
 *
 * @param origin The starting point of the ray.
 * @param direction The direction of the ray.
 * @return Ray The initialized ray with a normalized direction.
 */
Ray Ray_new(const Vector3D origin, const Vector3D direction) {
    // Normalize the direction vector
    Vector3D normalized_direction = Vector3D_normalize(direction);
    const Ray ray = {origin, normalized_direction};
    return ray;
}

/**
 * @brief Calculates a point along the ray at a specified distance from the origin.
 *
 * The function computes the point by adding the scaled direction vector to the origin.
 *
 * @param ray The ray to calculate the point on.
 * @param distance The distance from the ray's origin to the point.
 * @return Vector3D The calculated point along the ray at the specified distance.
 */
Vector3D Ray_pointAt(const Ray *ray, double distance) {
    // Calculate the point at the specified distance along the ray
    const Vector3D scaled_direction = Vector3D_scale(ray->direction, distance);
    return Vector3D_add(ray->origin, scaled_direction);
}
