#include "RayTracing.hpp"
#include "Renderer.h"
#include "raytracers/MetalRaytracer.hpp"
#include "raytracers/SequentialRayTracer.hpp"
#include "SFML/System/Vector2.hpp"

using namespace RayTracing;

int main(int argc, char *argv[]) {
    sf::Vector2u windowSize = sf::Vector2u(800, 600);

    Scene scene = Scene::loadFromFile(SCENE_BASE_DIR "scene.json");

    RayTracer *raytracer = new SequentialRayTracer(windowSize.x, windowSize.y, 3, 1);
    Image *uvTest = raytracer->uvTest();
    Image *raytraced = raytracer->raytrace(scene);

    auto renderer = Renderer(windowSize);

    while (renderer.isOpen()) {
        renderer.processEvents();
        renderer.draw(raytraced);
    }

    return 0;
}
