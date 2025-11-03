#pragma once

#include <entity.h>
#include <memory>
#include <texture.h>

class GroundPlane final : public Entity
{
public:
    explicit GroundPlane(const std::string& texturePath);

    void Draw(Shader& shader) override;

private:
    unsigned int m_VAO{};
    std::unique_ptr<Texture> m_Texture;
};