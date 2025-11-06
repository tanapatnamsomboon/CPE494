#pragma once

#include "shader.h"
#include <glm/glm.hpp>

enum class RenderLayer
{
    Sky = 0,
    Ground,
    Opaque,
    AlphaCutout,
    Transparent,
    UI
};

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void Update(float) {}
    virtual void Draw(Shader& shader) = 0;

    [[nodiscard]] virtual const glm::vec3& GetPosition() const { return m_Position; }
    [[nodiscard]] virtual const glm::vec3& GetRotation() const { return m_Rotation; }
    [[nodiscard]] virtual const glm::vec3& GetScale() const    { return m_Scale; }
    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
    void SetScale(const glm::vec3& scale)       { m_Scale = scale; }

    [[nodiscard]] virtual RenderLayer GetRenderLayer() const { return m_RenderLayer; }
    void SetRenderLayer(RenderLayer layer) { m_RenderLayer = layer; }

    [[nodiscard]] virtual bool HasCollision() const { return m_HasCollision; }
    [[nodiscard]] virtual float GetCollisionRadius() const { return m_CollisionRadius; }
    void SetCollisionRadius(float radius) { m_CollisionRadius = radius; }

protected:
    glm::vec3 m_Position{ 0.0f };
    glm::vec3 m_Rotation{ 0.0f };
    glm::vec3 m_Scale{ 1.0f };

    RenderLayer m_RenderLayer = RenderLayer::Opaque;

    bool m_HasCollision{ false };
    float m_CollisionRadius{ 1.0f };

};