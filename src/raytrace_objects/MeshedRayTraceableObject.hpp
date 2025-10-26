#pragma once
#include <nlohmann/json.hpp>
#include <vector>

#include "RayTracableObject.hpp"

namespace RayTracing {
    struct Mesh {
    public:
        std::vector<Vec3> vertices;
        std::vector<int> indices;
        std::vector<Vec3> normals;

        unsigned numTriangles;

        std::pair<std::vector<unsigned>, std::vector<unsigned> > split(float value, Vec3::Direction axis);
    };


    struct SerializableMeshedRayTraceableObject : public SerializableRayTraceableObject {
        std::string fileName;
        Vec3 scale;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableMeshedRayTraceableObject, color, position, rotation, fileName, scale)
    };

    class MeshedRayTraceableObject : public RayTraceableObject {
    public:
        std::string fileName;
        Mesh *mesh = nullptr;

        NestedBoundingBox nestedBoundingBox;

        MeshedRayTraceableObject() : RayTraceableObject({}, {}, Vec3(1), {}) {
        };

        MeshedRayTraceableObject(const RGBf &color, const Vec3 &position, const Vec3 &rotation,
                                 const std::string &fileName)
            : RayTraceableObject(color, position, {1, 1, 1}, rotation),
              fileName(fileName) {
        }

        MeshedRayTraceableObject(const SerializableMeshedRayTraceableObject &obj) : RayTraceableObject(
                obj.color, obj.position, obj.scale, obj.rotation), fileName(obj.fileName) {
        }

        void loadMesh(const std::string &baseDir);

        void updateBoundingBox() override;

        void updateNestedBoundingBox(unsigned maxTrianglesPerBox);

    private:
        NestedBoundingBox updateNestedBoundingBoxRecursive(const std::vector<int> &indices,
                                                           unsigned maxTrianglesPerBox);

        BoundingBox calculateBoundingBoxForIndices(const std::vector<int> &indices);
    };
}