#pragma once
#include <fstream>
#include <vector>

#include "simples.hpp"
#include <nlohmann/json.hpp>

namespace RayTracing {
    struct Mesh {
        public:
        std::vector<Vec3> points;
        std::vector<int> indices;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Mesh, points, indices)
    };

    struct Scene {
    public:
        std::vector<Mesh> objects;

    public:
        Scene() = default;

        static Scene loadFromFile(const std::string &path);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, objects)
    };
}
