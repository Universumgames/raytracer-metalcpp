#pragma once
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "RayTracableObject.hpp"

namespace RayTracing {
    /// Triangle mesh structure
    struct Mesh {
    public:
        std::vector<Vec3> vertices;
        std::vector<int> indices;
        std::vector<Vec3> normals;

        unsigned numTriangles;

        std::pair<std::vector<unsigned>, std::vector<unsigned> > split(float value, Vec3::Direction axis);
    };

    /// Serializable representation of a meshed ray traceable object
    struct SerializableMeshedRayTraceableObject : public SerializableRayTraceableObject {
        std::string fileName;
        Vec3 scale;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableMeshedRayTraceableObject, color, position, rotation, fileName, scale)
    };

    /// Ray traceable object represented by a triangle mesh
    class MeshedRayTraceableObject : public RayTraceableObject {
    public:
        std::string fileName;
        Mesh *mesh = nullptr;

        NestedBoundingBox *nestedBoundingBox = nullptr;

        MeshedRayTraceableObject() : RayTraceableObject({}, {}, Vec3(1), {}) {
        };

        MeshedRayTraceableObject(const RGBf &color, const Vec3 &position, const Vec3 &rotation,
                                 std::string fileName)
            : RayTraceableObject(color, position, {1, 1, 1}, rotation),
              fileName(std::move(fileName)) {
        }

        explicit MeshedRayTraceableObject(const SerializableMeshedRayTraceableObject &obj) : RayTraceableObject(
                obj.color, obj.position, obj.scale, obj.rotation.asDegreeToRadian()), fileName(obj.fileName) {
        }

        /**
         * Load the mesh from file in baseDir
         * @param baseDir Base directory where the mesh file is located
         */
        void loadMesh(const std::string &baseDir);

        /**
         * Calculate and update the bounding box of the meshed object
         */
        void updateBoundingBox() override;

        /**
         * Update the nested bounding box for spatial partitioning
         * @param maxTrianglesPerBox Maximum number of triangles allowed per bounding box
         */
        void updateNestedBoundingBox(unsigned maxTrianglesPerBox);

    private:
        /// Recursively update the nested bounding box
        NestedBoundingBox *updateNestedBoundingBoxRecursive(const std::vector<int> &indices,
                                                            unsigned maxTrianglesPerBox,
                                                            unsigned triangleCount);

        /// Calculate the bounding box for given indices
        [[nodiscard]] BoundingBox calculateBoundingBoxForIndices(const std::vector<int> &indices) const;
    };
}