#pragma once
#include "vectors.hpp"

namespace RayTracing {


    struct RayTraceableObject {
        Color color;
        Vec3 position;
        Vec4 orientation;
    };

    struct Sphere: RayTraceableObject {
        float radius{};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Sphere, position, color, radius)
    };

    struct Mesh {
    public:
        std::vector<Vec3> points;
        std::vector<int> indices;
        std::vector<Vec3> normals;

        unsigned numTriangles;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Mesh, points, indices)
    };

    struct MeshedRayTraceableObject : public RayTraceableObject {
        std::string fileName;
        Mesh* mesh = nullptr;

        void loadMesh(const std::string& stlFilePath);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(MeshedRayTraceableObject, fileName, color, position, orientation)

    };
}
