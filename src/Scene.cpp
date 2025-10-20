#include "Scene.hpp"

#include <iostream>
#include <stl_reader.h>

namespace RayTracing {

    Scene Scene::loadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + path);
        }
        nlohmann::json data = nlohmann::json::parse(file);
        Scene scene = data.get<Scene>();
        for (auto& object : scene.objects) {
            object.loadMesh();
        }
        return scene;
    }

    void RayTraceableObject::loadMesh() {
        mesh = new Mesh();
        try {
            stl_reader::StlMesh <float, unsigned int> stl_mesh (std::string(SCENE_BASE_DIR) + fileName);

            for(size_t itri = 0; itri < stl_mesh.num_tris(); ++itri) {
                std::vector<Vec3> vertices;
                for(size_t icorner = 0; icorner < 3; ++icorner) {
                    const float* c = stl_mesh.tri_corner_coords (itri, icorner);
                    mesh->points.emplace_back(c[0], c[1], c[2]);
                    mesh->indices.emplace_back(mesh->indices.size());
                    vertices.emplace_back(c[0], c[1], c[2]);
                }

                const float *n = stl_mesh.tri_normal(itri);
                mesh->normals.emplace_back(n[0], n[1], n[2]);
                mesh->numTriangles++;
            }
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

}
