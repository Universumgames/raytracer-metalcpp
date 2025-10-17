#include <stdlib.h>
#include <stdio.h>

#include <SFML/Graphics.h>

#include "../include/vector3d.h"
#include "../include/plane.h"
#include "../include/sphere.h"
#include "../include/bitmap.h"
#include "../include/scene.h"
#include "../include/camera.h"
#include "../include/light.h"
#include "../include/raytracer.h"

#define BITMAP_WIDTH 800
#define BITMAP_HEIGHT 800
#define MAX_DEPTH 5

/**
 * @brief Creates a scene, a raytracer and uses it to render the scene.
 */
int main()
{
    int exitStatus = EXIT_SUCCESS;

    sfRenderWindow* window = NULL;
    sfImage* bitmap = NULL;
    sfTexture* texture = NULL;
    sfSprite* sprite = NULL;

    // ----------------------------------------------------------------------
    // initialize the graphical user interface

    // create the window
    sfVideoMode mode = {BITMAP_WIDTH, BITMAP_HEIGHT, 32};
    window = sfRenderWindow_create (mode, "CCaster",  sfDefaultStyle, NULL);
    sfRenderWindow_setFramerateLimit(window, 50);

    // load a sprite to display the main image
    bitmap = sfImage_create(BITMAP_WIDTH, BITMAP_HEIGHT);
    texture = sfTexture_createFromImage(bitmap, NULL);
    sprite = sfSprite_create();
    sfSprite_setTexture( sprite, texture, sfFalse );
    sfVector2f spritePos = { 0, 0 };
    sfSprite_setPosition( sprite, spritePos );
    sfRenderWindow_drawSprite(window, sprite, NULL);

    // Create a camera
    const Vector3D camera_position = Vector3D_new(-7.5, 0, -7.5);
    const Vector3D camera_lookat = Vector3D_new(0, 0, 0);
    const Vector3D camera_up = Vector3D_new(0, 1, 0);
    const Camera camera = Camera_new(camera_position, camera_lookat, camera_up);

    // Create a scene
    Scene scene = Scene_new();

    // Add 3 spheres in a row to the scene
    for(uint8_t x = 0;x < 3; x++) {
        const Sphere sphere = Sphere_new(Vector3D_new(-5 + x * 5, 0, 0), 2, x % 2 == 0 ? sfRed : sfBlue);
        Scene_add_sphere(&scene, sphere);
    }

    // Add a ground plane below the spheres to the scene
    Plane plane = Plane_new(Vector3D_new(0,1,0),2, sfGreen);
    Scene_add_plane(&scene, plane);

    // Add a light source to the scene
    Light light = Light_new(Vector3D_new(0, -20, -10), sfWhite);
    Scene_add_light(&scene, light);

    // Create a raytracer for rendering the scene
    const Raytracer raytracer = Raytracer_new(camera, scene, MAX_DEPTH);
    // Render the scene to the bitmap
    Raytracer_render(&raytracer, bitmap);
    //antialias the bitmap
    Bitmap_antialias_bitmap(bitmap);

    // ----------------------------------------------------------------------
    // start the main loop

    while (sfRenderWindow_isOpen( window )) {
        char stepString [10];

        // get new events - and process them one by one
        sfEvent event;
        while ( sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close( window );
        }

        // update image
        sfTexture_updateFromImage(texture, bitmap, 0, 0);
        sfRenderWindow_drawSprite(window, sprite, NULL);

        sfRenderWindow_display(window);
    }

    // the following code needs to be executed
    // always at the end of the program
    // to clean up everything
    exitProgram:

    // clean up graphical user interface
    if (bitmap != NULL) sfImage_destroy( bitmap );
    if (texture != NULL) sfTexture_destroy( texture );
    if (sprite != NULL) sfSprite_destroy( sprite );
    if (window != NULL) sfRenderWindow_destroy( window );

    // exit
    return exitStatus;
}