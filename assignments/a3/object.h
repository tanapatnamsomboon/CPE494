#pragma once

#include <model.h>

class Object
{
public:
    Object(Model* model, glm::vec3 pos, float r = 1.0f);

    void Draw(const Shader& shader);

public:
    Model* m_Model;
    glm::vec3 m_Position{ 0, 0, 0 };
    float m_Radius = 1.0f;
};