#pragma once
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Camera.hpp"
#include "raytrace_objects/LightSource.hpp"
#include "raytrace_objects/MeshedRayTraceableObject.hpp"
#include "raytrace_objects/Sphere.hpp"

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
        Camera* camera;
        std::vector<MeshedRayTraceableObject*> objects;
        std::vector<Sphere*> spheres;
        std::vector<LightSource*> lights;
        std::string fileName{};

    public:
        Scene() = default;

        static Scene loadFromFile(const std::string &path);

    };
}
