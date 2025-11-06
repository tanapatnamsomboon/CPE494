#include "ground_plane.h"

#include <shape.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>

GroundPlane::GroundPlane(const std::string& texturePath)
{
    m_VAO = Shape::CreatePlane();
    m_Texture = std::make_unique<Texture>(texturePath.c_str());

    m_RenderLayer = RenderLayer::Ground;
    m_HasCollision = true;
    m_CollisionRadius = 0.0f;
}

void GroundPlane::Draw(Shader& shader)
{
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, m_Position);
    model = glm::scale(model, m_Scale);

    shader.SetMat4("uModel", model);
    shader.SetIdentityBones();
    shader.SetBool("hasTexture", true);
    m_Texture->Bind(0);
    shader.SetInt("texture_diffuse1", 0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
