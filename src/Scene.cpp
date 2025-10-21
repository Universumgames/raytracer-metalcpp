#include "Scene.hpp"

namespace RayTracing {

    Scene Scene::loadFromFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + path);
        }
        nlohmann::json data = nlohmann::json::parse(file);
        Scene scene = data.get<Scene>();
        scene.fileName = path;
        std::string baseDir = path.substr(0, path.find_last_of('/'));
        for (auto& object : scene.objects) {
            object.loadMesh(baseDir + "/" + object.fileName);
        }
        return scene;
    }

}
