#include "../include/sphere.h"

/**
 * @brief Initializes a new sphere with a specified center and radius.
 *
 * @param center The center point of the sphere.
 * @param radius The radius of the sphere.
 * @param color The color of the sphere.
 * @return Sphere The initialized sphere.
 */
Sphere Sphere_new(const Vector3D center, const double radius, const sfColor color) {
    const Sphere sphere = {center, radius, color, 0.5, 5};
    return sphere;
}

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
Sphere Sphere_new_with_specular(const Vector3D center, const double radius, const sfColor color, const double specular_intensity, const int specular_exponent) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    sphere.color = color;
    sphere.specular_intensity = specular_intensity;
    sphere.specular_exponent = specular_exponent;
    return sphere;
}

/**
 * @brief Calculates the intersection of a ray with the sphere.
 *
 * This function checks if a given ray intersects the sphere and, if so,
 * calculates the distance from the ray's origin to the intersection point.
 *
 * @param sphere The sphere with which to check intersection.
 * @param ray The ray to check for intersection with the sphere.
 * @param out_distance Pointer to a double where the distance to the nearest
 * intersection point will be stored if an intersection occurs.
 * @return int Returns 1 if the ray intersects the sphere, 0 otherwise.
 */
int Sphere_intersect(const Sphere *sphere, const Ray *ray, double *out_distance) {
    const Vector3D oc = Vector3D_subtract(ray->origin, sphere->center);
    const double a = Vector3D_dot(ray->direction, ray->direction);
    const double b = 2.0 * Vector3D_dot(oc, ray->direction);
    const double c = Vector3D_dot(oc, oc) - sphere->radius * sphere->radius;
    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 0; // No intersection
    } else {
        // Calculate the nearest intersection point
        const double sqrt_discriminant = sqrt(discriminant);
        const double t1 = (-b - sqrt_discriminant) / (2.0 * a);
        const double t2 = (-b + sqrt_discriminant) / (2.0 * a);

        // Select the closest positive intersection
        if (t1 >= 0) {
            *out_distance = t1;
        } else if (t2 >= 0) {
            *out_distance = t2;
        } else {
            return 0; // Intersection is behind the ray origin
        }
        return 1;
    }
}

/**
 * @brief Calculates the normal vector at a given point on the sphere's surface.
 *
 * The normal vector is a unit vector pointing outward from the sphere's center
 * through the point on the surface.
 *
 * @param sphere The sphere for which to calculate the normal.
 * @param point The point on the sphere's surface.
 * @return Vector3D The normalized normal vector at the specified point on the sphere.
 */
Vector3D Sphere_normal(const Sphere *sphere, const Vector3D point) {
    const Vector3D normal = Vector3D_subtract(point, sphere->center);
    return Vector3D_normalize(normal);
}
