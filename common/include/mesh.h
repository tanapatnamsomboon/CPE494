#pragma once

#include "shader.h"
#include <glm/glm.hpp>
#include <string>

constexpr int MAX_BONE_INFLUENCE = 4;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    int   BoneIDs[MAX_BONE_INFLUENCE];
    float Weights[MAX_BONE_INFLUENCE];
};

struct TextureData
{
    unsigned int ID;
    std::string Type;
    std::string Path;
    bool HasAlpha = false;
};

struct MaterialColor
{
    glm::vec3 Color = glm::vec3(1.0f);
    bool HasColor = false;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);

    void Draw(const Shader& shader) const;

private:
    void SetupMesh();

public:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<TextureData> m_Textures;
    MaterialColor m_Color;

private:
    unsigned int m_VAO{}, m_VBO{}, m_EBO{};
};