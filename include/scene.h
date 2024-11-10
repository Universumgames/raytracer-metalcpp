#ifndef SCENE_H
#define SCENE_H

#include "plane.h"
#include "sphere.h"
#include "light.h"
#include <stddef.h>

#define MAX_SPHERES 10   ///< Maximum number of spheres in the scene.
#define MAX_PLANES 10    ///< Maximum number of planes in the scene.
#define MAX_LIGHTS 10     ///< Maximum number of lights in the scene.

/**
 * @brief Structure representing a scene containing objects and lights.
 *
 * The scene contains arrays of spheres, planes, and lights with fixed maximum capacities.
 */
typedef struct {
    Sphere spheres[MAX_SPHERES];   ///< Array of spheres in the scene.
    size_t sphere_count;           ///< Number of spheres in the scene.

    Plane planes[MAX_PLANES];      ///< Array of planes in the scene.
    size_t plane_count;            ///< Number of planes in the scene.

    Light lights[MAX_LIGHTS];      ///< Array of lights in the scene.
    size_t light_count;            ///< Number of lights in the scene.
} Scene;

/**
 * @brief Initializes a new empty scene.
 *
 * Sets initial object counts to zero.
 *
 * @return Scene The initialized scene.
 */
Scene Scene_new();

/**
 * @brief Adds a sphere to the scene.
 *
 * @param scene Pointer to the Scene struct.
 * @param sphere The sphere to add to the scene.
 * @return int Returns 1 on success, 0 if the scene is full.
 */
int Scene_add_sphere(Scene* scene, Sphere sphere);

/**
 * @brief Adds a plane to the scene.
 *
 * @param scene Pointer to the Scene struct.
 * @param plane The plane to add to the scene.
 * @return int Returns 1 on success, 0 if the scene is full.
 */
int Scene_add_plane(Scene* scene, Plane plane);

/**
 * @brief Adds a light to the scene.
 *
 * @param scene Pointer to the Scene struct.
 * @param light The light to add to the scene.
 * @return int Returns 1 on success, 0 if the scene is full.
 */
int Scene_add_light(Scene* scene, Light light);

#endif
