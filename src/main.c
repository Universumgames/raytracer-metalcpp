#include <stdio.h>
#include "../include/vector3d.h"
#include "../include/plane.h"
#include "../include/sphere.h"
#include "../include/bitmap.h"
#include "../include/scene.h"
#include "../include/camera.h"
#include "../include/light.h"
#include "../include/raytracer.h"

#define BITMAP_WIDTH 2000
#define BITMAP_HEIGHT 2000
#define MAX_DEPTH 5

/**
 * @brief Creates a scene, a raytracer and uses it to render the scene.
 */
uint32_t main()
{
    printf("Raytracer started\r\n");

    // Create a camera
    const Vector3D camera_position = Vector3D_new(-7.5, 0, -7.5);
    const Vector3D camera_lookat = Vector3D_new(0, 0, 0);
    const Vector3D camera_up = Vector3D_new(0, 1, 0);
    const Camera camera = Camera_new(camera_position, camera_lookat, camera_up);

    // Create a scene
    printf("Creating scene\r\n");
    Scene scene = Scene_new();

    // Add 3 spheres in a row to the scene
    for(uint8_t x = 0;x < 3; x++) {
        const Sphere sphere = Sphere_new(Vector3D_new(-5 + x * 5, 0, 0), 2, x % 2 == 0 ? COLOR_RED : COLOR_BLUE);
        Scene_add_sphere(&scene, sphere);
    }

    // Add a ground plane below the spheres to the scene
    Plane plane = Plane_new(Vector3D_new(0,1,0),2, COLOR_DARK_GREEN);
    Scene_add_plane(&scene, plane);

    // Add a light source to the scene
    Light light = Light_new(Vector3D_new(0, -20, -10), COLOR_WHITE);
    Scene_add_light(&scene, light);

    printf("Scene created\r\n");

    // Create a raytracer for rendering the scene
    const Raytracer raytracer = Raytracer_new(camera, scene, MAX_DEPTH);

    // Create a bitmap
    printf("Creating bitmap with %dx%d resolution\r\n",BITMAP_WIDTH, BITMAP_HEIGHT);
    Bitmap bitmap = Bitmap_create(BITMAP_WIDTH, BITMAP_HEIGHT);
    printf("Bitmap created\r\n");

    printf("Rendering scene with %d rays\r\n", BITMAP_WIDTH * BITMAP_HEIGHT);
    // Render the scene to the bitmap
    Raytracer_render(&raytracer, &bitmap);
    printf("Scene rendered\r\n");

    //antialias the bitmap
    printf("Antialias the bitmap\r\n");
    Bitmap_antialias_bitmap(&bitmap);
    printf("Bitmap antialiazed\r\n");

    printf("Saving bitmap\r\n");
    // Save the rendered image to a bitmap file
    Bitmap_save("rendered_scene.bmp", bitmap);
    printf("Bitmap saved\r\n");

    // Clean up
    Bitmap_free(bitmap);

    return 0;
}