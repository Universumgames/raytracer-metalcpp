#include "../include/plane.h"

/**
 * @brief Initializes a new plane given a normal vector and an offset.
 *
 * The normal vector is normalized to ensure it has unit length.
 *
 * @param normal The normal vector of the plane (will be normalized).
 * @param offset The offset distance from the origin along the normal.
 * @param color The color of the plane.
 * @return Plane The initialized plane.
 */
Plane Plane_new(const Vector3D normal, const double offset, const Color color) {
    // Normalize the normal vector to ensure it has unit length
    const Vector3D normalized_normal = Vector3D_normalize(normal);
    Plane plane = {normalized_normal, offset, color};
    return plane;
}

/**
 * @brief Calculates the intersection of a ray with the plane.
 *
 * Computes the point where a given ray intersects the plane, if any.
 *
 * @param plane The plane with which to check intersection.
 * @param ray The ray to check for intersection.
 * @param out_distance Pointer to a double where the distance from the ray's origin
 * to the intersection point will be stored if an intersection occurs.
 * @return int Returns 1 if an intersection occurs, 0 otherwise.
 */
int Plane_intersect(const Plane *plane, const Ray *ray, double *out_distance) {
    // Calculate the dot product of the plane's normal and the ray's direction
    const double denom = Vector3D_dot(plane->normal, ray->direction);

    // If the denominator is near zero, the ray is parallel to the plane
    if (fabs(denom) < 1e-6) {
        return 0; // No intersection
    }

    // Compute the distance from the ray origin to the intersection point
    const Vector3D origin_to_plane = Vector3D_scale(plane->normal, plane->offset);
    const Vector3D diff = Vector3D_subtract(origin_to_plane, ray->origin);
    const double distance = Vector3D_dot(diff, plane->normal) / denom;

    // Check if the intersection is in the ray's positive direction
    if (distance >= 0) {
        *out_distance = distance;
        return 1;
    }

    return 0; // Intersection is behind the ray origin
}

/**
 * @brief Returns the normal vector of the plane.
 *
 * Since the plane's normal is constant, this function simply returns the plane's normal vector.
 *
 * @param plane The plane whose normal vector is to be retrieved.
 * @return Vector3D The normal vector of the plane.
 */
Vector3D Plane_normal(const Plane *plane) {
    return plane->normal;
}
