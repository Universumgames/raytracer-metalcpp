#pragma once

#include <iostream>
#include <vector>

#include "RayTracer.hpp"

#define TIMING_START(name) auto start_##name = std::chrono::high_resolution_clock::now();
#define TIMING_END(name) auto end_##name = std::chrono::high_resolution_clock::now(); \
auto duration_##name = std::chrono::duration_cast<std::chrono::milliseconds>(end_##name - start_##name);\
auto duration_##name##_millis = duration_##name.count();

#define TIMING_MILLIS(name) (duration_##name##_millis)


#define TIMING_LOG(name, component, operation)\
    RaytracingTimer::getInstance()->logDuration(this, component, TIMING_MILLIS(name), operation);

#define TIMING_LOG_RAYTRACER(raytracer, name, component, operation)\
RaytracingTimer::getInstance()->logDuration(raytracer, component, TIMING_MILLIS(name), operation);

#define TIMING_GET_DURATION(raytracer, component) \
    RaytracingTimer::getInstance()->getDuration(raytracer, component)


class RaytracingTimer {
public:
    enum class Component {
        SCENE_LOADING,
        ENCODING,
        RAYTRACING,
    };

private:
    RaytracingTimer() = default;

    static RaytracingTimer *instance;

    static std::map<Component, std::string> componentNames;

    std::map<std::string, std::map<Component, std::chrono::high_resolution_clock::time_point> > startTimings;
    std::map<std::string, std::map<Component, double> > timings;

public:
    static RaytracingTimer *getInstance();

    void start(RayTracing::RayTracer *tracer, Component component);

    void end(RayTracing::RayTracer *tracer, Component component);

    void log(RayTracing::RayTracer *tracer, Component component, std::string humanText);

    void logDuration(RayTracing::RayTracer *tracer, Component component, double duration, std::string humanText);

    double getDuration(RayTracing::RayTracer *tracer, Component component);
};