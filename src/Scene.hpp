#pragma once
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "RayTracableObject.hpp"

namespace RayTracing {

    struct Scene {
    public:
        std::vector<MeshedRayTraceableObject> objects;
        std::vector<Sphere> spheres;
        std::string fileName{};

    public:
        Scene() = default;

        static Scene loadFromFile(const std::string &path);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, objects, spheres)
    };
}
