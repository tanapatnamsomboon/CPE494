#pragma once

#include <glm/glm.hpp>

namespace Collision
{
    inline bool SphereIntersect(const glm::vec3& a, float ra,
                                const glm::vec3& b, float rb)
    {
        return glm::length(a - b) < (ra + rb);
    }
}