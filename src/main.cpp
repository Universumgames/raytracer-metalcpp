#include <iostream>

#include "RayTracing.hpp"
#include "Renderer.h"
#include "raytracers/MetalRaytracer.hpp"
#include "raytracers/SequentialRayTracer.hpp"
#include "SFML/System/Vector2.hpp"

using namespace RayTracing;

void benchmarkRaytracer(RayTracer* raytracer, const std::string &name, const Scene& scene, bool deleteTracer = true) {
    auto start = std::chrono::high_resolution_clock::now();
    Image* raytraced = raytracer->raytrace(scene);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[" << name << "] Laufzeit: " << duration.count() << " ms\n";

    std::ofstream timeLog;
    timeLog.open("../timeLog.csv", std::ios::app);
    timeLog << name << "," << PLATFORM_NAME << "," << ARCHITECTURE << "," << scene.fileName << "," << duration.count() << "\n";
    timeLog.close();

    delete raytraced;

    if (deleteTracer) {
        delete raytracer;
    }
}

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
