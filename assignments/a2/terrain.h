#pragma once

#include <vector>
#include <string>

class Terrain
{
public:
    explicit Terrain(const std::string& heightmapPath, float heightScale = 10.0f, bool invert = false);
    ~Terrain();

    void Draw() const;
    void SetHeightScale(float scale);
    void SetInvert(bool invert);
    float GetHeightScale() const { return m_HeightScale; }
private:
    void LoadHeightmap(const std::string& path);
    void GenerateMesh();

private:
    bool m_Inverted;
    unsigned int m_VAO{}, m_VBO{}, m_EBO{};
    int m_Width{}, m_Height{};
    float m_HeightScale;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
};
