#pragma once
#include "Color.hpp"
#include "matrices.hpp"
#include "vectors.hpp"

#define deg2rad(deg) (deg * M_PI / 180.0)

namespace RayTracing {
    struct RayTraceableObject {
        RGBf color;
        Vec3 position;
        Quaternion quaternion;

        [[nodiscard]] Mat3x3 getRotationMatrix() const;

        [[nodiscard]] Vec3 getTranslatedRotatedPoint(const Mat3x3 &rotationMatrix, const Vec3 &point) const;

        static Vec3 getRotatedNormal(const Mat3x3 &rotationMatrix, const Vec3 &normal);

        void rotateEuler(Vec3 rad);
    };

    struct Sphere : RayTraceableObject {
        float radius{};

        Sphere() = default;

        Sphere(RGBA8 color, const Vec3& position, const Quaternion& quaternion, float radius) : RayTraceableObject(color, position,
            quaternion), radius(radius) {
        }

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
        Mesh *mesh = nullptr;

        void loadMesh(const std::string &stlFilePath);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(MeshedRayTraceableObject, fileName, color, position, quaternion)
    };
}
