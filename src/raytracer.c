#include <stdio.h>
#include <float.h>
#include "../include/raytracer.h"

/**
 * @brief Initializes a new raytracer with a specified camera, scene, and maximum depth.
 * @see https://en.wikipedia.org/wiki/Ray_tracing_(graphics)
 *
 * @param camera The camera to use for ray tracing.
 * @param scene The scene to render.
 * @param max_depth The maximum recursion depth for reflections.
 * @return Raytracer The initialized raytracer.
 */
Raytracer Raytracer_new(const Camera camera, const Scene scene, const int max_depth) {
    Raytracer raytracer;
    raytracer.camera = camera;
    raytracer.scene = scene;
    raytracer.max_depth = max_depth;
    return raytracer;
}

#define CLAMP_COLOR_COMPONENT(value) \
((value) < 0 ? 0 : ((value) > 255 ? 255 : (uint8_t)(value)))

/**
 * @brief Calculates the color of the closest object that a ray intersects in the scene.
 *
 * @todo 1: Add possibility to render more than one light in the scene
 * @todo 2: Add possibility to render shadows in the scene (and "fix" the specular lightning)
 * @todo 3: Add possibility to render reflections in the scene
 * @todo 4: Use the "config service" to be able to load scenes from config files
 * @todo 5: <optional> add refraction to the scene
 * @todo 6: <optional> add materials (textures) to the raytracer
 * @todo 7: <optional> add other primitives to the raytracer
 * @todo 8: <optional> use the c graphics library "Simple and Fast Multimedia Library" (CFSML)
 *          to render raytracing in real time
 *
 * @param raytracer Pointer to the Raytracer struct.
 * @param ray The ray to trace.
 * @param depth The current recursion depth.
 * @return Color The color of the closest object intersected by the ray, or background color if no intersection.
 */
Color Raytracer_trace_ray(const Raytracer *raytracer, const Ray ray, const int depth) {
    if (depth > raytracer->max_depth) {
        return COLOR_BLACK; // Return black for rays that reach max recursion depth
    }

    double nearest_distance = DBL_MAX;
    Sphere nearest_sphere;
    Plane nearest_plane;
    Objecttype nearest_objecttype = NONE;

    // Get the first light in the scene (assuming there's at least one light)
    const Vector3D light_position = raytracer->scene.lights[0].position;
    const Color light_color = raytracer->scene.lights[0].color;

    // Check for intersections with spheres in the scene
    for (size_t i = 0; i < raytracer->scene.sphere_count; i++) {
        double distance;
        if (Sphere_intersect(&raytracer->scene.spheres[i], &ray, &distance) && distance < nearest_distance) {
            nearest_distance = distance;
            nearest_sphere = raytracer->scene.spheres[i];
            nearest_objecttype = SPHERE;
        }
    }

    // Check for intersections with planes in the scene
    for (size_t i = 0; i < raytracer->scene.plane_count; i++) {
        double distance;
        if (Plane_intersect(&raytracer->scene.planes[i], &ray, &distance) && distance < nearest_distance) {
            nearest_distance = distance;
            nearest_plane = raytracer->scene.planes[i];
            nearest_objecttype = PLANE;
        }
    }

    // Get the data from the nearest intersected object needed for determining the color
    Vector3D intersection_point;
    Vector3D normal;
    Color object_color;
    double specular_intensity = 0;  //default intensity value for objects with no specular intensity level
    double specular_exponent = 1;   //default exponent value for objects with no specular exponent
    switch (nearest_objecttype) {
        case SPHERE: {
            intersection_point = Ray_pointAt(&ray, nearest_distance);
            normal = Sphere_normal(&nearest_sphere, intersection_point);
            object_color = nearest_sphere.color;
            specular_intensity = nearest_sphere.specular_intensity;
            specular_exponent = nearest_sphere.specular_exponent;
            break;
        }
        case PLANE: {
            intersection_point = Ray_pointAt(&ray, nearest_distance);
            normal = Vector3D_scale(Plane_normal(&nearest_plane),-1);
            object_color = nearest_plane.color;
            break;
        }
        case NONE:
        default: {
            return COLOR_BLACK; // Return background color if no object is hit
        }
    }

    // Calculate the direction to the light source
    Vector3D light_direction = Vector3D_normalize(Vector3D_subtract(light_position, intersection_point));

    // Calculate diffuse intensity based on the angle between the normal and light direction
    // @see (German): https://de.wikipedia.org/wiki/Diffuses_Licht
    double diffuse_intensity = Vector3D_dot(normal, light_direction);
    if (diffuse_intensity < 0) diffuse_intensity = 0; // Clamp to zero if light is behind the surface

    // Calculate specular highlight
    Vector3D view_direction = Vector3D_normalize(Vector3D_subtract(raytracer->camera.position, intersection_point));
    Vector3D reflect_direction = Vector3D_reflect(Vector3D_scale(light_direction, -1), normal); // Reflect light direction around normal

    double dot = Vector3D_dot(view_direction, reflect_direction);
    if (dot < 0) {
        dot = 0;  // Clamp to avoid invalid pow calculation
    }

    double specular_intensity_calc = pow(dot, specular_exponent) * specular_intensity;

    Color final_color;
    final_color.r = CLAMP_COLOR_COMPONENT(
        object_color.r * (LIGHT_AMBIENT_COLOR.r / 255.0 * LIGHT_AMBIENT_INTENSITY) +  // Ambient
        object_color.r * diffuse_intensity +                                          // Diffuse
        specular_intensity_calc * light_color.r                                       // Specular
    );

    final_color.g = CLAMP_COLOR_COMPONENT(
        object_color.g * (LIGHT_AMBIENT_COLOR.g / 255.0 * LIGHT_AMBIENT_INTENSITY) +  // Ambient
        object_color.g * diffuse_intensity +                                          // Diffuse
        specular_intensity_calc * light_color.g                                       // Specular
    );

    final_color.b = CLAMP_COLOR_COMPONENT(
        object_color.b * (LIGHT_AMBIENT_COLOR.b / 255.0 * LIGHT_AMBIENT_INTENSITY) +  // Ambient
        object_color.b * diffuse_intensity +                                          // Diffuse
        specular_intensity_calc * light_color.b                                       // Specular
    );

    return final_color;
}

/**
 * @brief Renders the scene to a bitmap by tracing rays for each pixel in the bitmap.
 *
 * @param raytracer Pointer to the Raytracer struct.
 * @param bitmap Pointer to the Bitmap where the image will be rendered.
 */
void Raytracer_render(const Raytracer *raytracer, Bitmap *bitmap) {
    const int width = bitmap->width;
    const int height = bitmap->height;
    const double fov = 90.0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Compute the direction of the ray for the current pixel
            const Vector3D ray_direction = Camera_get_ray_direction(&raytracer->camera, x, y, width, height, fov);
            // Create the ray from the camera to the ray_direction
            const Ray ray = Ray_new(raytracer->camera.position, ray_direction);

            // Trace the ray and get the color for the current pixel
            const Color pixel_color = Raytracer_trace_ray(raytracer, ray, raytracer->max_depth);

            // Set the pixel color in the bitmap
            Bitmap_put_pixel(bitmap, x, y, pixel_color);
        }
    }
}