#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(const Shader& shader) const;

private:
    void SetupMesh();

public:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<Texture> m_Textures;
    unsigned int m_VAO;

private:
    unsigned int m_VBO, m_EBO;
};
