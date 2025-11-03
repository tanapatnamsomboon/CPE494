#include "object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(Model *model, glm::vec3 pos, float r)
    : m_Model(model), m_Position(pos), m_Radius(r)
{
}

void Object::Draw(const Shader& shader)
{
    auto model = m_Model->GetRootTransform();
    model = glm::translate(model, m_Position);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.SetMat4("uModel", model);
    m_Model->Draw(shader);
}
