#include <iostream>

#include "RayTracing.hpp"
#include "Renderer.h"
#include "raytracers/MetalRaytracer.hpp"
#include "raytracers/SequentialRayTracer.hpp"
#include "SFML/System/Vector2.hpp"

using namespace RayTracing;

#define BENCHMARK_LOG_FILE "../timeLog.csv"

Image *benchmarkRaytracer(RayTracer *raytracer, const Scene &scene, bool deleteImg = true, bool deleteTracer = true) {
    auto start = std::chrono::high_resolution_clock::now();
    Image *raytraced = raytracer->raytrace(scene);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[" << raytracer->identifier() << "] Laufzeit: " << duration.count() << " ms\n";

    bool exists = std::ifstream(BENCHMARK_LOG_FILE).good();
    std::ofstream timeLog;
    timeLog.open(BENCHMARK_LOG_FILE, std::ios::app);
    if (!exists) {
        timeLog << "Implementation,Platform,Architecture,Filename,Samples,Bounces,Duration(ms)" << std::endl;
    }
    timeLog << raytracer->identifier() << "," << PLATFORM_NAME << "," << ARCHITECTURE << "," << scene.fileName << "," <<
            raytracer->getSamplesPerPixel() << "," << raytracer->getBounces() << "," << duration.count()
            << std::endl;
    timeLog.close();

    if (deleteTracer) {
        delete raytracer;
    }
    if (deleteImg) {
        delete raytraced;
        return nullptr;
    }
    return raytraced;
}

int main(int argc, char *argv[]) {
    sf::Vector2u windowSize = sf::Vector2u(800, 600);

    Scene scene = Scene::loadFromFile(SCENE_BASE_DIR "scene.json");

    RayTracer *raytracer = new SequentialRayTracer(windowSize.x, windowSize.y, 3, 1);
    Image *uvTest = raytracer->uvTest();
    Image *raytraced = benchmarkRaytracer(raytracer, scene, false);

    auto test = Sphere({0, 0, 0, 0}, {0, 0, 0}, {}, 3);
    test.rotateEuler({0, 0, deg2rad(45)});
    Vec3 p = {0, 0, 1};
    Vec3 pn = test.getTranslatedRotatedPoint(test.getRotationMatrix(), p);


    auto imageHandler = new ImageHandler(windowSize);
    imageHandler->saveImage("raytraced.jpg", raytraced);

#ifndef RUNNING_CICD
    auto renderer = Renderer(windowSize, imageHandler);

    while (renderer.isOpen()) {
        renderer.processEvents();
        renderer.draw(raytraced);
    }
#endif

    return 0;
}
