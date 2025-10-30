#include <iostream>

#include "RayTracer.hpp"
#include "Renderer.h"
#include "raytracers/RayTracerFactory.hpp"
#include "argumentsResolver.hpp"
#include "timing.hpp"

using namespace RayTracing;

/**
 * Benchmark the given raytracer with the given scene and log the time taken to a CSV file
 * @param raytracer the raytracer implemenation to benchmark
 * @param scene the scene to raytrace
 * @param deleteImg whether to delete the resulting image after benchmarking
 * @param deleteTracer whether to delete the raytracer after benchmarking
 * @return the resulting image if deleteImg is false, nullptr otherwise
 */
Image *benchmarkRaytracer(RayTracer *raytracer, const Scene &scene, bool deleteImg = true, bool deleteTracer = true) {
    TIMING_START(raytrace)
    Image *raytraced = raytracer->raytrace(scene);
    TIMING_END(raytrace)
    TIMING_LOG(raytrace, "Benchmarking", raytracer->identifier() + " raytracing")

    bool exists = std::ifstream(benchmarkFile).good();
    std::ofstream timeLog;
    timeLog.open(benchmarkFile, std::ios::app);
    if (!exists) {
        timeLog << "Implementation,Platform,Architecture,Filename,Samples,Bounces,Triangles,Spheres,Duration(ms)" <<
                std::endl;
    }
    timeLog << raytracer->identifier() << "," << PLATFORM_NAME << "," << ARCHITECTURE << "," << scene.fileName << "," <<
            raytracer->getSamplesPerPixel() << "," << raytracer->getBounces() << "," << scene.totalTriangleCount() <<
            "," << scene.totalSphericalCount() << "," << TIMING_MILLIS(raytrace)
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
    decodeArguments(argc, argv);
    if (helped) {
        return 0;
    }

    auto imageHandler = new ImageHandler(windowSize);
    auto raytracerFactory = RayTracerFactory::init(windowSize, bounces, samples);
    auto *raytracer = sequential
                          ? raytracerFactory->getSequentialImplementation()
                          : raytracerFactory->getShaderImplementation();
    if (raytracer == nullptr) {
        std::cerr << "No implementation found for desired raytracer, using sequential implementation" << std::endl;
        raytracer = raytracerFactory->getSequentialImplementation();
    }
    std::cout << "Using raytracer implementation: " << raytracer->identifier() << std::endl;

    Scene scene = Scene::loadFromFile(sceneFile);

    if (renderTests) {
        Image *uvTest = raytracer->uvTest();
        imageHandler->saveImage("uvTest.jpg", uvTest);

        Image *rayTest = raytracer->rayTest(scene.camera);
        imageHandler->saveImage("rayTest.jpg", rayTest);

        delete uvTest;
        delete rayTest;
    }

    scene.prepareRender();

    Image *raytraced = benchmarkRaytracer(raytracer, scene, false, false);

    imageHandler->saveImage(outputFile, raytraced);

    std::cout << "[" << raytracer->identifier() << "] Rendered raytrace image from scene " << sceneFile << " to " <<
            outputFile << std::endl;

#ifndef RUNNING_CICD
    if (openWindow) {
        auto renderer = Renderer(windowSize, imageHandler);

        while (renderer.isOpen()) {
            renderer.processEvents();
            renderer.draw(raytraced);
        }
    }
#endif

    return 0;
}