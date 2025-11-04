#include "item_pickup.h"

#include <glm/gtc/matrix_transform.hpp>

ItemPickup::ItemPickup(Model* model, ItemId id, int count, const glm::vec3& position, std::string variant, float radius)
    : m_Model(model), m_Id(id), m_Count(count), m_Position(position), m_Variant(std::move(variant)), m_Radius(radius)
{
}

void ItemPickup::Draw(Shader& shader)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_Position);
    shader.SetMat4("uModel", model);
    shader.SetBool("uUseSkinning", false);
    if (m_Model)
        m_Model->Draw(shader);
}
