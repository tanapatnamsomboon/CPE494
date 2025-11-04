#pragma once

#include "entity/static_object.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class WorldSpawner
{
public:
    static std::vector<std::shared_ptr<StaticObject>>
    SpawnEnvironment(int treeCount = 30, int rockCount = 20, int grassCount = 40,
                     const glm::vec2& areaHalfExtents = { 50.0f, 50.0f });
};