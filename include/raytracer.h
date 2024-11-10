#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "scene.h"
#include "camera.h"
#include "bitmap.h"

typedef enum {
 NONE = 0,
 SPHERE = 1,
 PLANE = 2
} Objecttype;

/**
 * @brief Structure representing the raytracer, which contains a camera, a scene, and render settings.
 */
typedef struct {
    Camera camera;       ///< Camera from which rays are traced.
    Scene scene;         ///< The 3D scene containing objects and lights.
    int max_depth;       ///< Maximum depth for recursive ray tracing (reflection, etc.).
} Raytracer;

/**
 * @brief Initializes a raytracer with a specified camera, scene, and maximum depth.
 * @see https://en.wikipedia.org/wiki/Ray_tracing_(graphics)
 *
 * @param camera The camera to use for ray tracing.
 * @param scene The scene to render.
 * @param max_depth The maximum recursion depth for reflections.
 * @return Raytracer The initialized raytracer.
 */
Raytracer Raytracer_new(Camera camera, Scene scene, int max_depth);

/**
 * @brief Renders the scene to a bitmap by tracing rays for each pixel.
 *
 * @param raytracer Pointer to the Raytracer struct.
 * @param bitmap Pointer to the Bitmap where the image will be rendered.
 */
void Raytracer_render(const Raytracer *raytracer, Bitmap *bitmap);

#endif
