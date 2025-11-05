#pragma once
#include <iostream>
#include <string>

#include "math/vectors.hpp"
#include "raytracers/RayTracerFactory.hpp"

extern bool openWindow;
extern bool renderTests;
extern bool helped;
extern RayTracing::RayTracerType implementation;
extern std::string outputFile;
extern std::string sceneFile;
extern std::string benchmarkFile;
extern unsigned bounces;
extern unsigned samples;
extern RayTracing::Vec2u windowSize;

/**
 * Resolve command line arguments and set global variables accordingly
 * @param argc argument count
 * @param argv argument values
 */
inline void decodeArguments(int argc, char *argv[]) {
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
            std::cout << "\t--multi-threaded\t\t use the multi-threaded cpu raytracer implementation" << std::endl;
            std::cout << "\t--shader\t\t\t use the gpu raytracer implementation (default)" << std::endl;
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
            implementation = RayTracing::SEQUENTIAL;
        } else if (arg == "--multi-threaded") {
            implementation = RayTracing::MULTI_THREADED;
        } else if (arg == "--shader") {
            implementation = RayTracing::SHADER_BASED;
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
            windowSize = RayTracing::Vec2u(width, height);
            i += 2;
        }
    }
}
