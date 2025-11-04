#pragma once

#include "entity/item_pickup.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class ItemSpawner
{
public:
    static std::vector<std::shared_ptr<ItemPickup>>
    SpawnRandomItems(int count,
                     const glm::vec2& areaHalfExtents = { 40.0f, 40.0f },
                     float yHeight = 0.0f);
};