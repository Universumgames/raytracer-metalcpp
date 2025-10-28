#pragma once
#include <fstream>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "Camera.hpp"
#include "raytrace_objects/LightSource.hpp"
#include "raytrace_objects/MeshedRayTraceableObject.hpp"
#include "raytrace_objects/SphereRayTraceableObject.hpp"

namespace RayTracing {
    struct SerializableScene {
        SerializableCamera camera;
        std::vector<SerializableMeshedRayTraceableObject> objects;
        std::vector<SerializableSphere> spheres;
        std::vector<SerializableLightSource> lights;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(SerializableScene, camera, objects, spheres, lights)
    };

    struct Scene {
    public:
        Camera *camera = nullptr;
        std::vector<MeshedRayTraceableObject *> objects;
        std::vector<SphereRayTraceableObject *> spheres;
        std::vector<LightSource *> lights;
        std::string fileName{};

    public:
        Scene() = default;

        explicit Scene(Camera *camera, std::vector<MeshedRayTraceableObject *> objects = {},
                       std::vector<SphereRayTraceableObject *> spheres = {},
                       std::vector<LightSource *> lights = {}, std::string fileName = "")
            : camera(camera), objects(std::move(objects)), spheres(std::move(spheres)), lights(std::move(lights)),
              fileName(std::move(fileName)) {
        }

        /**
         * Load a scene from a file
         * @param path Path to the scene file
         * @return The loaded scene
         */
        static Scene loadFromFile(const std::string &path);

        /**
         * Get the total number of triangles in all meshed objects in the scene
         * @return Total triangle count
         */
        [[nodiscard]] unsigned totalTriangleCount() const {
            unsigned count = 0;
            for (const auto &obj: objects) {
                count += obj->mesh->numTriangles;
            }
            return count;
        }

        /**
         * Get the total number of spherical objects (spheres and lights) in the scene
         * @return Total spherical object count
         */
        [[nodiscard]] unsigned totalSphericalCount() const {
            return spheres.size() + lights.size();
        }
    };
}