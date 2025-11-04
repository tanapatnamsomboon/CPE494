#pragma once

#include <entity.h>
#include <model.h>

class StaticObject final : public Entity
{
public:
    StaticObject(Model* model, const glm::vec3& pos, float radius = 1.0f);

    void Draw(Shader& shader) override;

private:
    Model* m_Model;
    float  m_Radius;
};