#include <iostream>

#include "RayTracer.hpp"
#include "Renderer.h"
#include "raytracers/RayTracerFactory.hpp"
#include "argumentsResolver.hpp"
#include "timing.hpp"

using namespace RayTracing;

bool openWindow = true;
bool renderTests = false;
bool helped = false;
RayTracerType implementation = SHADER_BASED;
std::string outputFile = "./raytraced.jpg";
std::string sceneFile = "scene/scene_monkey.json";
std::string benchmarkFile = "../timeLog.csv";
unsigned bounces = 10;
unsigned samples = 20;
Vec2u windowSize = RayTracing::Vec2u(1920, 1440);
// auto windowSize = Vec2u(400, 300);

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
    TIMING_LOG_RAYTRACER(raytracer, raytrace, RaytracingTimer::Component::TOTAL_RAYTRACING, "Total raytracing time");

    bool exists = std::ifstream(benchmarkFile).good();
    std::ofstream timeLog;
    timeLog.open(benchmarkFile, std::ios::app);
    if (!exists) {
        timeLog <<
                "Implementation,Platform,Architecture,Filename," <<
                "Samples,Bounces,Rays," <<
                "Width,Height," <<
                "Triangles,Spheres," <<
                "Scene Loading(ms),Encoding(ms),Raytracing(ms),Decoding(ms),Total Duration (ms)," <<
                "Git Hash"
                <<
                std::endl;
    }
    timeLog << raytracer->identifier() << "," << PLATFORM_NAME << "," << ARCHITECTURE << "," << scene.fileName << "," <<
            raytracer->getSamplesPerPixel() << "," << raytracer->getBounces() << "," << raytracer->getRayCount() << ","
            <<
            raytracer->getWindowSize().getX() << "," << raytracer->getWindowSize().getY() << "," <<
            scene.totalTriangleCount() << "," << scene.totalSphericalCount() << "," <<
            TIMING_GET_DURATION(raytracer, RaytracingTimer::Component::SCENE_LOADING) << "," <<
            TIMING_GET_DURATION(raytracer, RaytracingTimer::Component::ENCODING) << "," <<
            TIMING_GET_DURATION(raytracer, RaytracingTimer::Component::RAYTRACING) << "," <<
            TIMING_GET_DURATION(raytracer, RaytracingTimer::Component::DECODING) << "," <<
            TIMING_GET_DURATION(raytracer, RaytracingTimer::Component::TOTAL_RAYTRACING) << "," <<
            GIT_COMMIT_HASH
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

/// get all scene files in the scene directory and validate the json structure
void sceneValidator() {
    auto files = std::filesystem::directory_iterator("scene/");
    for (const auto &file: files) {
        if (file.path().extension() == ".json") {
            try {
                auto scene = Scene::loadFromFile(file.path().string());
            } catch (std::runtime_error &e) {
                std::cerr << "Error loading scene " << file.path().string() << std::endl;
                std::cerr << e.what() << std::endl;
            }
            std::cout << "Validated scene file: " << file.path().string() << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    decodeArguments(argc, argv);
    if (helped) {
        return 0;
    }

    sceneValidator();

    auto imageHandler = new ImageHandler(windowSize);
    auto raytracerFactory = RayTracerFactory::init(windowSize, bounces, samples);
    auto *raytracer = raytracerFactory->getRayTracerByType(implementation);
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
