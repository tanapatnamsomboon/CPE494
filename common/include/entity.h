#pragma once

#include "shader.h"
#include <glm/glm.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void Update(float /*dt*/) {}
    virtual void Draw(Shader& shader) = 0;

    [[nodiscard]] glm::vec3 GetPosition() const { return m_Position; }
    [[nodiscard]] glm::vec3 GetRotation() const { return m_Rotation; }
    [[nodiscard]] glm::vec3 GetScale() const    { return m_Scale; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
    void SetScale(const glm::vec3& scale)       { m_Scale = scale; }

protected:
    glm::vec3 m_Position{ 0.0f };
    glm::vec3 m_Rotation{ 0.0f };
    glm::vec3 m_Scale{ 1.0f };
};