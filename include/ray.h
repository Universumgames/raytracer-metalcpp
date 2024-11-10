#ifndef RAY_H
#define RAY_H

#include "vector3d.h"

/**
 * @brief Structure representing a ray in 3D space.
 *
 * A ray has an origin (starting point) and a direction.
 */
typedef struct {
    Vector3D origin;    ///< The origin point of the ray.
    Vector3D direction; ///< The direction of the ray (should be a unit vector).
} Ray;

/**
 * @brief Initializes a new ray with a specified origin and direction.
 *
 * The direction vector will be normalized to ensure it has unit length.
 *
 * @param origin The starting point of the ray.
 * @param direction The direction of the ray.
 * @return Ray The initialized ray with a normalized direction.
 */
Ray Ray_new(Vector3D origin, Vector3D direction);

/**
 * @brief Calculates a point along the ray at a specified distance from the origin.
 *
 * @param ray The ray to calculate the point on.
 * @param distance The distance from the ray's origin to the point.
 * @return Vector3D The calculated point along the ray.
 */
Vector3D Ray_pointAt(const Ray *ray, double distance);

#endif
