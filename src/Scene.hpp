#pragma once
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Color.hpp"
#include "vectors.hpp"

namespace RayTracing {

    struct Sphere {
        Vec3 center;
        Color color;
        float radius;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Sphere, center, color, radius)
    };

    struct Mesh {
        public:
        std::vector<Vec3> points;
        std::vector<int> indices;
        std::vector<Vec3> normals;

        unsigned numTriangles;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Mesh, points, indices)
    };

    struct RayTraceableObject {
        std::string fileName;
        Mesh* mesh = nullptr;
        Color color;
        Vec3 position;
        Vec4 orientation;

        void loadMesh();

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(RayTraceableObject, fileName, color, position, orientation)
    };

    struct Scene {
    public:
        std::vector<RayTraceableObject> objects;
        std::vector<Sphere> spheres;

    public:
        Scene() = default;

        static Scene loadFromFile(const std::string &path);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Scene, objects, spheres)
    };
}
