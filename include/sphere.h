#ifndef SPHERE_H
#define SPHERE_H

#include "vector3d.h"
#include "ray.h"
#include "color.h"

/**
 * @brief Structure representing a sphere in 3D space.
 * @see: https://en.wikipedia.org/wiki/Sphere
 *
 * A sphere is defined by a center position and a radius.
 * The specular intensity can also be defined
 */
typedef struct {
 Vector3D center;                  ///< The center point of the sphere.
 double radius;                    ///< The radius of the sphere.
 Color color;                      ///< The base color of the sphere.
 double specular_intensity;  ///< Intensity of the specular reflection (strength of highlight).
 int specular_exponent;        ///< Shininess of the sphere (higher value = smaller, sharper highlight).
} Sphere;

/**
 * @brief Initializes a new sphere with a specified center and radius.
 *
 * @param center The center point of the sphere.
 * @param radius The radius of the sphere.
 * @param color The color of the sphere.
 * @return Sphere The initialized sphere.
 */
Sphere Sphere_new(Vector3D center, double radius, Color color);

/**
 * @brief Initializes a new sphere with a specified center, radius, color, and specular properties.
 *
 * @param center The center point of the sphere.
 * @param radius The radius of the sphere.
 * @param color The base color of the sphere.
 * @param specular_intensity The intensity of the specular reflection (strength of the highlight).
 * @param specular_exponent The shininess of the sphere (higher value = smaller, sharper highlight).
 * @return Sphere The initialized sphere.
 */
Sphere Sphere_new_with_specular(Vector3D center, double radius, Color color, double specular_intensity, int specular_exponent);

/**
 * @brief Calculates the intersection of a ray with the sphere.
 *
 * Determines whether a given ray intersects with the sphere and calculates
 * the distance to the closest intersection point if an intersection exists.
 *
 * @param sphere The sphere with which to check intersection.
 * @param ray The ray to check for intersection with the sphere.
 * @param out_distance Pointer to store the distance from the ray's origin to the intersection point.
 * @return int Returns 1 if the ray intersects the sphere, 0 otherwise.
 */
int Sphere_intersect(const Sphere *sphere, const Ray *ray, double *out_distance);

/**
 * @brief Calculates the normal vector at a given point on the sphere's surface.
 *
 * The normal vector is a unit vector pointing outward from the sphere's center
 * through the point on the surface.
 *
 * @param sphere The sphere for which to calculate the normal.
 * @param point The point on the sphere's surface.
 * @return Vector3D The normal vector at the specified point on the sphere.
 */
Vector3D Sphere_normal(const Sphere *sphere, Vector3D point);

#endif
