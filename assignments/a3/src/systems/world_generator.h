#pragma once

#include "entity/static_object.h"
#include <entity.h>
#include <model_manager.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <random>

namespace WorldGenerator
{
    inline std::vector<std::shared_ptr<Entity>> GenerateChunk(glm::vec2 origin, float size)
    {
        std::vector<std::shared_ptr<Entity>> result;

        std::mt19937 gen((int)(origin.x * 928371 + origin.y * 1928371));
        std::uniform_real_distribution pos(-size / 2, size / 2);
        std::uniform_real_distribution choose(0.0f, 1.0f);

        constexpr int density = 20;
        for (int i = 0; i < density; ++i)
        {
            glm::vec3 p{ origin.x + pos(gen), 0.0f, origin.y + pos(gen) };
            std::shared_ptr<Model> model;
            const float r = choose(gen);

            if (r < 0.4f)
                model = ModelManager::GetModel("assets/objects/pine_tree.fbx");
            else if (r < 0.7f)
                model = ModelManager::GetModel("assets/objects/rock_0.fbx");
            else
                model = ModelManager::GetModel("assets/objects/bush.fbx");

            auto obj = std::make_shared<StaticObject>(model.get(), p);
            // obj->SetCollision(true, r < 0.4f ? 3.0f : 2.0f);
            result.push_back(obj);
        }

        return result;
    }
}