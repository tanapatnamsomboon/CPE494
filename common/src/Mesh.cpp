#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
{
    m_Vertices = vertices;
    m_Indices = indices;
    m_Textures = textures;
    SetupMesh();
}

void Mesh::Draw(const Shader &shader) const
{
    unsigned int diffuseNr = 1;

    for (size_t i = 0; i < m_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name = m_Textures[i].type;
        std::string number = std::to_string(diffuseNr++);
        shader.SetInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)m_Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
