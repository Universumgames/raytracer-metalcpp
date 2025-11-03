#include "timing.hpp"

RaytracingTimer *RaytracingTimer::instance = nullptr;
std::map<RaytracingTimer::Component, std::string> RaytracingTimer::componentNames = {
    {Component::SCENE_LOADING, "Scene Loading"},
    {Component::ENCODING, "Encoding"},
    {Component::RAYTRACING, "Raytracing"},
};

RaytracingTimer *RaytracingTimer::getInstance() {
    if (instance == nullptr) {
        instance = new RaytracingTimer();
    }
    return instance;
}

void RaytracingTimer::start(RayTracing::RayTracer *tracer, Component component) {
    std::string tracerName = tracer->identifier();
    startTimings[tracerName][component] = std::chrono::high_resolution_clock::now();
}

void RaytracingTimer::end(RayTracing::RayTracer *tracer, Component component) {
    std::string tracerName = tracer->identifier();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto startTime = startTimings[tracerName][component];
    double duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    timings[tracerName][component] += duration;
}

void RaytracingTimer::log(RayTracing::RayTracer *tracer, Component component, std::string humanText) {
    std::string tracerName = tracer->identifier();
    double duration = timings[tracerName][component];

    std::cout << "[" << tracerName << "] " << ((humanText.empty()) ? componentNames[component] : humanText) << " took "
            << duration << " ms\n";
}

void RaytracingTimer::logDuration(RayTracing::RayTracer *tracer, Component component, double duration,
                                  std::string humanText) {
    std::string tracerName = tracer->identifier();
    timings[tracerName][component] += duration;

    std::cout << "[" << tracerName << "] " << ((humanText.empty()) ? componentNames[component] : humanText) << " took "
            << duration << " ms\n";
}

double RaytracingTimer::getDuration(RayTracing::RayTracer *tracer, Component component) {
    return timings[tracer->identifier()][component];
}
