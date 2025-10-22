#include "MeshedRayTraceableObject.hpp"

#include <iostream>
#include <stl_reader.h>

namespace RayTracing {
    void MeshedRayTraceableObject::loadMesh(std::string baseDir) {
        mesh = new Mesh();
        try {
            Vec3 minLoc = {INFINITY, INFINITY, INFINITY};
            Vec3 maxLoc = {-INFINITY, -INFINITY, -INFINITY};
            stl_reader::StlMesh<float, unsigned int> stl_mesh(baseDir + "/" + fileName);

            for (size_t itri = 0; itri < stl_mesh.num_tris(); ++itri) {
                std::vector<Vec3> vertices;
                for (size_t icorner = 0; icorner < 3; ++icorner) {
                    const float *c = stl_mesh.tri_corner_coords(itri, icorner);
                    mesh->vertices.emplace_back(c[0], c[1], c[2]);
                    mesh->indices.emplace_back(mesh->indices.size());
                    Vec3 v = Vec3(c[0], c[1], c[2]);
                    minLoc = Vec3(std::min(minLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()), std::min(minLoc.getZ(), v.getZ()));
                    maxLoc = Vec3(std::max(maxLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()), std::max(minLoc.getZ(), v.getZ()));
                    vertices.push_back(v);
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
        for (auto& v : mesh->vertices) {
            minLoc = Vec3(std::min(minLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()), std::min(minLoc.getZ(), v.getZ()));
            maxLoc = Vec3(std::max(maxLoc.getX(), v.getX()), std::min(minLoc.getY(), v.getY()), std::max(minLoc.getZ(), v.getZ()));
        }
        this->boundingBox = {minLoc, maxLoc};
    }
}
