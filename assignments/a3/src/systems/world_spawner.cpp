#include "world_spawner.h"

#include <model_manager.h>
#include <random>
#include <iostream>

std::vector<std::shared_ptr<StaticObject>>
WorldSpawner::SpawnEnvironment(int treeCount, int rockCount, int grassCount, const glm::vec2& areaHalfExtents)
{
    std::vector<std::shared_ptr<StaticObject>> result;
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution distX(-areaHalfExtents.x, areaHalfExtents.x);
    std::uniform_real_distribution distZ(-areaHalfExtents.y, areaHalfExtents.y);

    auto spawnType = [&](const std::vector<std::string>& variants, int count) {
        std::uniform_int_distribution distVar(0, (int)variants.size() - 1);
        for (int i = 0; i < count; ++i) {
            const std::string& variant = variants[distVar(gen)];
            std::string path = "assets/objects/" + variant + ".fbx";
            auto model = ModelManager::GetModel(path);
            glm::vec3 pos{ distX(gen), 0.0f, distZ(gen) };
            result.push_back(std::make_shared<StaticObject>(model.get(), pos));
        }
    };

    spawnType({ "pine_tree", "oak_tree" }, treeCount);

    spawnType({ "rock_0" }, rockCount);

    spawnType({ "bush", "grass", "fern" }, grassCount);

    std::cout << "[WorldSpawner] Spawned " << result.size() << " environment objects.\n";
    return result;
}
