#include <iostream>

#include "RayTracer.hpp"
#include "Renderer.h"
#include "raytracers/RayTracerFactory.hpp"

using namespace RayTracing;

bool openWindow = true;
bool renderTests = true;
bool helped = false;
bool sequential = false;
std::string outputFile = "raytraced.jpg";
std::string sceneFile = "scene/scene.json";
std::string benchmarkFile = "../timeLog.csv";

Image *benchmarkRaytracer(RayTracer *raytracer, const Scene &scene, bool deleteImg = true, bool deleteTracer = true) {
    auto start = std::chrono::high_resolution_clock::now();
    Image *raytraced = raytracer->raytrace(scene);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[" << raytracer->identifier() << "] Laufzeit: " << duration.count() << " ms\n";

    bool exists = std::ifstream(benchmarkFile).good();
    std::ofstream timeLog;
    timeLog.open(benchmarkFile, std::ios::app);
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

void decodeArguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            helped = true;
            std::cout << "Command line arguments:" << std::endl;
            std::cout << "\t-h or --help\t opens this help page" << std::endl;
            std::cout << "\t--no-window\t no window opens" << std::endl;
            std::cout << "\t-of <file>\t specify raytraced file path" << std::endl;
            std::cout << "\t--no-tests\t no test images are rendered" << std::endl;
            std::cout << "\t-s <json file>\t specify path to scene file" << std::endl;
            std::cout << "\t-b <file>\t specify benchmark csv file" << std::endl;
            std::cout << "\t--sequential\t use the sequential raytracer implementation instead of the gpu" << std::endl;
        } else if (arg == "--no-window") {
            openWindow = false;
        }else if (arg == "-of") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for -of" << std::endl;
            }
            outputFile = argv[i + 1];
            i++;
        }else if (arg == "--no-tests") {
            renderTests = false;
        }else if (arg == "-s") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for -s" << std::endl;
            }
            sceneFile = argv[i + 1];
            i++;
        }else if (arg == "-b") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for -b" << std::endl;
            }
            benchmarkFile = argv[i + 1];
            i++;
        } else if (arg == "--sequential") {
            sequential = true;
        }
    }
}

int main(int argc, char *argv[]) {
    decodeArguments(argc, argv);
    if (helped) {
        return 1;
    }

    auto windowSize = Vec2u(800, 600);

    auto imageHandler = new ImageHandler(windowSize);
    auto raytracerFactory = RayTracerFactory::init(windowSize, 3, 1);
    auto *raytracer = sequential
                          ? raytracerFactory->getSequentialImplementation()
                          : raytracerFactory->getShaderImplementation();
    std::cout << "Using raytracer implementation: " << raytracer->identifier() << std::endl;

    Scene scene = Scene::loadFromFile(sceneFile);

    if (renderTests) {
        Image *uvTest = raytracer->uvTest();
        Image *rayTest = raytracer->rayTest(scene.camera);

        imageHandler->saveImage("uvTest.jpg", uvTest);
        imageHandler->saveImage("rayTest.jpg", rayTest);
    }

    Image *raytraced = benchmarkRaytracer(raytracer, scene, false, false);

    imageHandler->saveImage(outputFile, raytraced);

    std::cout << "[" << raytracer->identifier() << "] Rendered raytrace image from scene " << sceneFile << " to " << outputFile << std::endl;

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
