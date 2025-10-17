#ifndef PLANE_H
#define PLANE_H

#include <SFML/Graphics/Color.h>

#include "vector3d.h"
#include "ray.h"

/**
 * @brief Structure representing a plane in 3D space.
 *
 * The plane is defined by a normal vector and an offset from the origin.
 */
typedef struct {
    Vector3D normal;  ///< The normal vector of the plane (must be unit length).
    double offset;    ///< The distance from the origin along the normal.
    sfColor color;      ///< The color of the plane.
} Plane;


/**
 * @brief Initializes a plane given a normal vector and an offset.
 *
 * @param normal The normal vector of the plane (will be normalized if not unit length).
 * @param offset The distance from the origin along the normal.
 * @param color The color of the plane.
 * @return Plane The initialized plane.
 */
Plane Plane_new(Vector3D normal, double offset, sfColor color);

/**
 * @brief Calculates the intersection of a ray with the plane.
 *
 * @param plane The plane with which to check intersection.
 * @param ray The ray to check for intersection with the plane.
 * @param out_distance Pointer to store the distance from ray origin to intersection point.
 * @return int Returns 1 if the ray intersects the plane, 0 otherwise.
 */
int Plane_intersect(const Plane *plane, const Ray *ray, double *out_distance);

/**
 * @brief Returns the normal vector of the plane.
 *
 * Since the plane's normal is constant, it simply returns the plane's normal vector.
 *
 * @param plane The plane whose normal vector is to be retrieved.
 * @return Vector3D The normal vector of the plane.
 */
Vector3D Plane_normal(const Plane *plane);

#endif
