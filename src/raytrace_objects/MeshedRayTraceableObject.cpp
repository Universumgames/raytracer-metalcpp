#include "MeshedRayTraceableObject.hpp"

#include <iostream>
#include <stl_reader.h>

namespace RayTracing {
    std::pair<std::vector<unsigned>, std::vector<unsigned> > Mesh::split(float value, Vec3::Direction axis) {
        std::vector<unsigned> indicesLeft = {};
        std::vector<unsigned> indicesRight = {};
        for (unsigned indice: indices) {
            Vec3 vertex = vertices[indice];
            if (vertex.getValue(axis) < value) {
                indicesLeft.push_back(indice);
            } else {
                indicesRight.push_back(indice);
            }
        }
        return {indicesLeft, indicesRight};
    }


    void MeshedRayTraceableObject::loadMesh(const std::string &baseDir) {
        mesh = new Mesh();
        try {
            Vec3 minLoc = {INFINITY, INFINITY, INFINITY};
            Vec3 maxLoc = {-INFINITY, -INFINITY, -INFINITY};
            stl_reader::StlMesh<float, unsigned int> stl_mesh(baseDir + "/" + fileName);

            std::vector<Vec3> vertices;
            for (size_t ivert = 0; ivert < stl_mesh.num_vrts(); ++ivert) {
                const float *c = stl_mesh.vrt_coords(ivert);
                Vec3 v = Vec3(c[0], c[1], c[2]);
                mesh->vertices.push_back(v);
                minLoc = Vec3(std::min(minLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()),
                                  std::min(minLoc.getZ(), v.getZ()));
                maxLoc = Vec3(std::max(maxLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()),
                              std::max(minLoc.getZ(), v.getZ()));
            }
            for (size_t itri = 0; itri < stl_mesh.num_tris(); ++itri) {
                for (size_t icorner = 0; icorner < 3; ++icorner) {
                    const unsigned index = stl_mesh.tri_corner_ind(itri, icorner);
                    mesh->indices.emplace_back(index);
                }
                const float *n = stl_mesh.tri_normal(itri);
                mesh->normals.emplace_back(n[0], n[1], n[2]);
                mesh->numTriangles++;
            }
            this->boundingBox = {minLoc, maxLoc};
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void MeshedRayTraceableObject::updateBoundingBox() {
        Vec3 minLoc = {INFINITY, INFINITY, INFINITY};
        Vec3 maxLoc = {-INFINITY, -INFINITY, -INFINITY};
        for (auto &v: mesh->vertices) {
            minLoc = Vec3(std::min(minLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()),
                          std::min(minLoc.getZ(), v.getZ()));
            maxLoc = Vec3(std::max(maxLoc.getX(), v.getX()), std::max(maxLoc.getY(), v.getY()),
                          std::max(maxLoc.getZ(), v.getZ()));
        }
        this->boundingBox = {minLoc, maxLoc};
    }

    void MeshedRayTraceableObject::updateNestedBoundingBox(unsigned maxTrianglesPerBox) {
        this->nestedBoundingBox = updateNestedBoundingBoxRecursive(mesh->indices, maxTrianglesPerBox);
    }

    NestedBoundingBox MeshedRayTraceableObject::updateNestedBoundingBoxRecursive(
        const std::vector<int> &indices, unsigned maxTrianglesPerBox) {
        if (indices.size() < maxTrianglesPerBox) {
            return NestedBoundingBox{
                calculateBoundingBoxForIndices(indices), indices, nullptr, nullptr, 0, Vec3::X_AXIS
            };
        }
        BoundingBox innerBoundingBox = calculateBoundingBoxForIndices(indices);
        // get longest axis to split along
        Vec3::Direction splitAxis = Vec3::X_AXIS;
        float length = 0;
        for (auto axis: {Vec3::X_AXIS, Vec3::Y_AXIS, Vec3::Z_AXIS}) {
            float axisLength = innerBoundingBox.maxPos.getValue(axis) - innerBoundingBox.minPos.getValue(axis);
            if (axisLength > length) {
                length = axisLength;
                splitAxis = axis;
            }
        }

        // split indices along axis center
        std::vector<int> indicesLeft = {};
        std::vector<int> indicesRight = {};
        float splitValue = innerBoundingBox.minPos.getValue(splitAxis) + (length / 2.0f);
        for (auto index: indices) {
            float axisValue = mesh->vertices[index].getValue(splitAxis);
            if (axisValue < splitValue) {
                indicesLeft.push_back(index);
            } else {
                indicesRight.push_back(index);
            }
        }

        return {
            calculateBoundingBoxForIndices(indices), indices,
            new NestedBoundingBox(updateNestedBoundingBoxRecursive(indicesLeft, maxTrianglesPerBox)),
            new NestedBoundingBox(updateNestedBoundingBoxRecursive(indicesRight, maxTrianglesPerBox)),
            splitValue, splitAxis
        };
    }

    BoundingBox MeshedRayTraceableObject::calculateBoundingBoxForIndices(const std::vector<int> &indices) const {
        Vec3 minLoc = {INFINITY, INFINITY, INFINITY};
        Vec3 maxLoc = {-INFINITY, -INFINITY, -INFINITY};
        for (auto index: indices) {
            Vec3 v = mesh->vertices[index];
            minLoc = Vec3(std::min(minLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()),
                          std::min(minLoc.getZ(), v.getZ()));
            maxLoc = Vec3(std::max(maxLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()),
                          std::max(minLoc.getZ(), v.getZ()));
        }
        return {minLoc, maxLoc};
    }
}