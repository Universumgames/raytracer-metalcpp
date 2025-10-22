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
    };

    struct SerializableMeshedRayTraceableObject: public SerializableRayTraceableObject{
        std::string fileName;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableMeshedRayTraceableObject, color, position, rotation, fileName)
    };

    class MeshedRayTraceableObject : public RayTraceableObject {
    public:
        std::string fileName;
        Mesh *mesh = nullptr;

        MeshedRayTraceableObject() : RayTraceableObject({}, {}, {}) {
        };

        MeshedRayTraceableObject(const RGBf &color, const Vec3 &position, const Quaternion &quaternion,
                                 const std::string &fileName)
            : RayTraceableObject(color, position, quaternion),
              fileName(fileName) {
        }

        MeshedRayTraceableObject(const SerializableMeshedRayTraceableObject &obj) : RayTraceableObject(
            obj.color, obj.position, Quaternion::fromEulerDegree(obj.rotation)), fileName(obj.fileName) {
        }

        void loadMesh(std::string baseDir);

        void updateBoundingBox() override;
    };
}
