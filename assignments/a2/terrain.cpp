#include "terrain.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <iostream>
#include <algorithm>

Terrain::Terrain(const std::string& heightmapPath, float heightScale, bool invert)
    : m_Inverted(invert), m_HeightScale(heightScale)
{
    LoadHeightmap(heightmapPath);
    GenerateMesh();
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Terrain::Draw() const
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Terrain::SetHeightScale(float scale)
{
    m_HeightScale = scale;
}

void Terrain::SetInvert(bool invert)
{
    m_Inverted = invert;
}

void Terrain::LoadHeightmap(const std::string& path)
{
    int channels;
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, 1);
    if (!data)
    {
        std::cerr << "Failed to load heightmap: " << path << std::endl;
        return;
    }

    m_Vertices.resize(m_Width * m_Height * 8);

    auto GetHeight = [&](int x, int z) -> float {
        x = std::clamp(x, 0, m_Width - 1);
        z = std::clamp(z, 0, m_Height - 1);
        float h = data[z * m_Width + x] / 255.0f;
        if (m_Inverted) h = 1.0f - h;
        return h * m_HeightScale;
    };

    for (int z = 0; z < m_Height; ++z)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            int i = (z * m_Width + x) * 8;
            float y = GetHeight(x, z);

            float fx = (float)x - m_Width / 2.0f;
            float fz = (float)z - m_Height / 2.0f;
            m_Vertices[i + 0] = fx;
            m_Vertices[i + 1] = y;
            m_Vertices[i + 2] = fz;

            m_Vertices[i + 3] = 0.0f;
            m_Vertices[i + 4] = 1.0f;
            m_Vertices[i + 5] = 0.0f;

            m_Vertices[i + 6] = (float)x / (m_Width  - 1);
            m_Vertices[i + 7] = (float)z / (m_Height - 1);
        }
    }

    for (int z = 1; z < m_Height - 1; ++z)
    {
        for (int x = 1; x < m_Width - 1; ++x)
        {
            float hL = GetHeight(x - 1, z);
            float hR = GetHeight(x + 1, z);
            float hD = GetHeight(x, z - 1);
            float hU = GetHeight(x, z + 1);

            glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));

            int i = (z * m_Width + x) * 8;
            m_Vertices[i + 3] = normal.x;
            m_Vertices[i + 4] = normal.y;
            m_Vertices[i + 5] = normal.z;
        }
    }

    stbi_image_free(data);
}

void Terrain::GenerateMesh()
{
    for (int z = 0; z < m_Height - 1; ++z)
    {
        for (int x = 0; x < m_Width - 1; ++x)
        {
            unsigned int topLeft = z * m_Width + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * m_Width + x;
            unsigned int bottomRight = bottomLeft + 1;

            m_Indices.insert(m_Indices.end(), {
                topLeft, bottomLeft, topRight,
                topRight, bottomLeft, bottomRight
            });
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
