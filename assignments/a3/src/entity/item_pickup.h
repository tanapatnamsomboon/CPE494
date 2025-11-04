#pragma once

#include "systems/item_id.h"
#include <entity.h>
#include <shader.h>
#include <model.h>
#include <glm/glm.hpp>

class ItemPickup final : public Entity
{
public:
    ItemPickup(
        Model*           model,
        ItemId           id,
        int              count,
        const glm::vec3& position,
        std::string      variant  = "",
        float            radius   = 0.6f
    );

    void Update(float) override {}
    void Draw(Shader& shader) override;

    [[nodiscard]] const glm::vec3& GetPosition() const override { return m_Position; }

    [[nodiscard]] float  GetRadius() const { return m_Radius; }
    [[nodiscard]] ItemId GetItem() const   { return m_Id; }
    [[nodiscard]] int    GetCount() const  { return m_Count; }

    [[nodiscard]] const std::string& GetVariant() const { return m_Variant; }

private:
    Model* m_Model{};
    ItemId m_Id{ ItemId::None };
    int    m_Count{ 1 };
    glm::vec3 m_Position{ 0 };
    std::string m_Variant;
    float  m_Radius{ 0.6f };
};