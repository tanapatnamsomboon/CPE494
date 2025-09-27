#pragma once

#include <vector>
#include <string>
#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const std::string& path);

    void Draw(const Shader& shader) const;

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;

private:
    std::vector<Texture> m_LoadedTextures;
};
