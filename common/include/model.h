#pragma once

#include <assimp/scene.h>
#include <map>
#include <string>
#include <vector>

#include "mesh.h"
#include "shader.h"
#include "animdata.h"

class Model
{
public:
    explicit Model(const std::string& path, bool gamma = false);

    void Draw(const Shader& shader) const;

    std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }
    [[nodiscard]] glm::mat4 GetRootTransform() const { return m_RootTransform; }

    [[nodiscard]] bool HasTransparency() const { return m_HasTransparency; }

private:
    void LoadModel(const std::string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

    static unsigned int TextureFromFile(const char* path, TextureData& outTex);
    static void InitVertexBoneData(Vertex& vertex);
    static void AssignBoneWeight(Vertex& vertex, int boneId, float weight);
    void PopulateBoneWeights(std::vector<Vertex>& vertices, aiMesh* mesh);

private:
    std::vector<Mesh> m_Meshes;
    std::vector<TextureData> m_LoadedTextures;
    std::map<std::string, BoneInfo> m_BoneInfoMap;

    glm::mat4 m_RootTransform;

    std::string m_Directory;
    bool m_GammaCorrection{ false };
    bool m_HasTransparency{ false };
    int m_BoneCounter{ 0 };
};
