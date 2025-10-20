#include "Scene.hpp"

namespace RayTracing {

    Scene Scene::loadFromFile(const std::string& path) {
        std::ifstream file(path);
        nlohmann::json data = nlohmann::json::parse(file);

        return data.get<Scene>();
    }
}
