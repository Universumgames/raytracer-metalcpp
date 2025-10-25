#pragma once
#include <fstream>
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
        Camera *camera;
        std::vector<MeshedRayTraceableObject *> objects;
        std::vector<SphereRayTraceableObject *> spheres;
        std::vector<LightSource *> lights;
        std::string fileName{};

    public:
        Scene() = default;

        Scene(Camera *camera, std::vector<MeshedRayTraceableObject *> objects = {},
              std::vector<SphereRayTraceableObject *> spheres = {},
              std::vector<LightSource *> lights = {}, const std::string &fileName = "")
            : camera(camera), objects(objects), spheres(spheres), lights(lights), fileName(fileName) {
        }

        static Scene loadFromFile(const std::string &path);
    };
}