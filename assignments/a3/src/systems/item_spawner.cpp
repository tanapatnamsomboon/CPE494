#include "item_spawner.h"

#include "item_database.h"
#include "systems/item_id.h"
#include <model_manager.h>
#include <random>
#include <iostream>

std::vector<std::shared_ptr<ItemPickup>> ItemSpawner::SpawnRandomItems(
    int count, const glm::vec2& areaHalfExtents, float yHeight)
{
    std::vector<std::shared_ptr<ItemPickup>> result;
    if (count <= 0) return result;

    const std::vector candidates = {
        ItemId::Apple, ItemId::Branch, ItemId::Bottle, ItemId::Bandage
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution distX(-areaHalfExtents.x, areaHalfExtents.x);
    std::uniform_real_distribution distZ(-areaHalfExtents.y, areaHalfExtents.y);
    std::uniform_int_distribution  distId(0, (int)candidates.size() - 1);

    for (int i = 0; i < count; i++)
    {
        ItemId id = candidates[distId(gen)];
        const auto& def = ItemDatabase::Get(id);
        std::string variant;
        if (def.Variants.empty())
        {
            variant = def.Name;
        }
        else
        {
            std::uniform_int_distribution distVariant(0, (int)def.Variants.size() - 1);
            variant = def.Variants[distVariant(gen)];
        }
        std::string path = "assets/objects/" + variant + ".fbx";
        auto model = ModelManager::GetModel(path);

        glm::vec3 pos{ distX(gen), yHeight, distZ(gen) };
        result.push_back(std::make_shared<ItemPickup>(model.get(), id, 1, pos, variant));
    }

    std::cout << "[ItemSpawner] Spawned " << result.size() << " items.\n";
    return result;
}
