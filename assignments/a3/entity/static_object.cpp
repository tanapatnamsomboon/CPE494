#include "static_object.h"

#include <glm/gtc/matrix_transform.hpp>

StaticObject::StaticObject(Model *model, const glm::vec3& pos, const float radius)
    : m_Model(model), m_Radius(radius)
{
    m_Position = pos;
}

void StaticObject::Draw(Shader& shader)
{
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, m_Position);
    shader.SetMat4("uModel", model);
    shader.SetIdentityBones();
    shader.SetBool("uUseSkinning", false);
    m_Model->Draw(shader);
}
