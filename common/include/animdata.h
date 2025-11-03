#pragma once

#include <glm/glm.hpp>

struct BoneInfo
{
    int ID;
    glm::mat4 Offset{ 1.0f };
};