#include "Scene.hpp"

namespace RayTracing {

    Scene Scene::loadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + path);
        }
        nlohmann::json data = nlohmann::json::parse(file);
        auto serializableScene  = data.get<SerializableScene>();

        Scene scene;
        scene.fileName = path;
        scene.camera = new Camera(serializableScene.camera);
        std::string baseDir = path.substr(0, path.find_last_of('/'));
        for (auto& object : serializableScene.objects) {
            auto loadedObj = new MeshedRayTraceableObject(object);
            loadedObj->loadMesh(baseDir);
            scene.objects.push_back(loadedObj);
        }
        for (auto& sphere : serializableScene.spheres) {
            auto loadedSphere = new Sphere(sphere);
            scene.spheres.push_back(loadedSphere);
        }
        for (auto& light : serializableScene.lights) {
            auto loadedLight = new LightSource(light);
            scene.lights.push_back(loadedLight);
        }
        return scene;
    }

}
