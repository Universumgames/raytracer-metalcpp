#include "../include/scene.h"
#include <stdio.h>

/**
 * @brief Initializes a new empty scene.
 *
 * Sets initial counts to zero for each object type.
 *
 * @return Scene The initialized scene.
 */
Scene Scene_new() {
    Scene scene;
    scene.sphere_count = 0;
    scene.plane_count = 0;
    scene.light_count = 0;
    return scene;
}

/**
 * @brief Adds a sphere to the scene if there is space.
 *
 * @param scene Pointer to the Scene struct.
 * @param sphere The sphere to add to the scene.
 * @return int Returns 1 on success, 0 if the scene's sphere capacity is full.
 */
int Scene_add_sphere(Scene* scene, const Sphere sphere) {
    if (scene->sphere_count < MAX_SPHERES) {
        scene->spheres[scene->sphere_count] = sphere;
        scene->sphere_count = scene->sphere_count + 1;
        return 1; // Success
    }
    return 0; // Scene full
}

/**
 * @brief Adds a plane to the scene if there is space.
 *
 * @param scene Pointer to the Scene struct.
 * @param plane The plane to add to the scene.
 * @return int Returns 1 on success, 0 if the scene's plane capacity is full.
 */
int Scene_add_plane(Scene* scene, const Plane plane) {
    if (scene->plane_count < MAX_PLANES) {
        scene->planes[scene->plane_count] = plane;
        scene->plane_count = scene->plane_count + 1;
        return 1; // Success
    }
    return 0; // Scene full
}

/**
 * @brief Adds a light to the scene if there is space.
 *
 * @param scene Pointer to the Scene struct.
 * @param light The light to add to the scene.
 * @return int Returns 1 on success, 0 if the scene's light capacity is full.
 */
int Scene_add_light(Scene* scene, const Light light) {
    if (scene->light_count < MAX_LIGHTS) {
        scene->lights[scene->light_count] = light;
        scene->light_count = scene->light_count + 1;
        return 1; // Success
    }
    return 0; // Scene full
}