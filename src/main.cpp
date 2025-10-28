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
unsigned bounces = 3;
unsigned samples = 1;
Vec2u windowSize = Vec2u(800, 600);

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
        timeLog << "Implementation,Platform,Architecture,Filename,Samples,Bounces,Triangles,Spheres,Duration(ms)" <<
                std::endl;
    }
    timeLog << raytracer->identifier() << "," << PLATFORM_NAME << "," << ARCHITECTURE << "," << scene.fileName << "," <<
            raytracer->getSamplesPerPixel() << "," << raytracer->getBounces() << "," << scene.totalTriangleCount() <<
            "," << scene.totalSphericalCount() << "," << duration.count()
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
            std::cout << "\t-h or --help\t\t\t opens this help page" << std::endl;
            std::cout << "\t--no-window\t\t\t no window opens" << std::endl;
            std::cout << "\t-of <file>\t\t\t specify raytraced file path (default: " << outputFile << ")" << std::endl;
            std::cout << "\t--no-tests\t\t\t no test images are rendered" << std::endl;
            std::cout << "\t-s <json file>\t\t\t specify path to scene file (default: " << sceneFile << ")" <<
                    std::endl;
            std::cout << "\t-b <file>\t\t\t specify benchmark csv file (default: " << benchmarkFile << ")" << std::endl;
            std::cout << "\t--sequential\t\t\t use the sequential raytracer implementation instead of the gpu" <<
                    std::endl;
            std::cout << "\t--bounces <num>\t\t\t specify number of bounces (default: " << bounces << ")" << std::endl;
            std::cout << "\t--samples <num>\t\t\t specify number of samples per pixel (default: " << samples << ")" <<
                    std::endl;
            std::cout << "\t--window-size <width> <height>\t specify window size (default: " << windowSize.getX() << "x"
                    << windowSize.getY() << ")" << std::endl;
        } else if (arg == "--no-window") {
            openWindow = false;
        } else if (arg == "-of") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for -of" << std::endl;
            }
            outputFile = argv[i + 1];
            i++;
        } else if (arg == "--no-tests") {
            renderTests = false;
        } else if (arg == "-s") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for -s" << std::endl;
            }
            sceneFile = argv[i + 1];
            i++;
        } else if (arg == "-b") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for -b" << std::endl;
            }
            benchmarkFile = argv[i + 1];
            i++;
        } else if (arg == "--sequential") {
            sequential = true;
        } else if (arg == "--bounces") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for --bounces" << std::endl;
            }
            bounces = std::stoi(argv[i + 1]);
            i++;
        } else if (arg == "--samples") {
            if (argc < i + 1) {
                std::cerr << "Missing argument for --samples" << std::endl;
            }
            samples = std::stoi(argv[i + 1]);
            i++;
        } else if (arg == "--window-size") {
            if (argc < i + 2) {
                std::cerr << "Missing argument for --window-size" << std::endl;
            }
            unsigned width = std::stoi(argv[i + 1]);
            unsigned height = std::stoi(argv[i + 2]);
            windowSize = Vec2u(width, height);
            i += 2;
        }
    }
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
