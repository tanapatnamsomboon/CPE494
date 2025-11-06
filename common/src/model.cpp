#include "Model.h"

#include "assimp_glm_helpers.h"
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <iostream>
#include <filesystem>

Model::Model(const std::string& path, const bool gamma)
    : m_GammaCorrection(gamma)
{
    LoadModel(path);
}

void Model::Draw(const Shader& shader) const
{
    for (const auto& mesh : m_Meshes)
        mesh.Draw(shader);
}

void Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[ASSIMP] ERROR: " << importer.GetErrorString() << std::endl;
        return;
    }

    m_RootTransform = AssimpGLMHelpers::ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation);

    m_Directory = path.substr(0, path.find_last_of("/\\"));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh processed = ProcessMesh(mesh, scene);
        m_Meshes.push_back(processed);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        ProcessNode(node->mChildren[i], scene);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureData> textures;

    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};
        InitVertexBoneData(vertex);
        vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
        vertex.Normal = mesh->HasNormals() ? AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]) : glm::vec3(0.0f);

        if (mesh->mTextureCoords[0])
            vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        else
            vertex.TexCoords = { 0.0f, 0.0f };

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    auto normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    auto heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    for (const auto& tex : textures)
    {
        if (tex.HasAlpha)
            m_HasTransparency = true;
    }

    PopulateBoneWeights(vertices, mesh);

    return { vertices, indices, textures };
}

void Model::InitVertexBoneData(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.BoneIDs[i] = -1;
        vertex.Weights[i] = 0.0f;
    }
}

void Model::AssignBoneWeight(Vertex& vertex, int boneId, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.BoneIDs[i] < 0)
        {
            vertex.BoneIDs[i] = boneId;
            vertex.Weights[i] = weight;
            return;
        }
    }
}

void Model::PopulateBoneWeights(std::vector<Vertex>& vertices, aiMesh* mesh)
{
    if (mesh->mNumBones == 0)
        return;

    for (int boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
    {
        int boneId = -1;
        std::string boneName = mesh->mBones[boneIdx]->mName.C_Str();

        if (!m_BoneInfoMap.contains(boneName))
        {
            BoneInfo info;
            info.ID = m_BoneCounter;
            info.Offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIdx]->mOffsetMatrix);
            m_BoneInfoMap[boneName] = info;
            boneId = m_BoneCounter++;
        }
        else
            boneId = m_BoneInfoMap[boneName].ID;

        for (unsigned weightIndex = 0; weightIndex < mesh->mBones[boneIdx]->mNumWeights; ++weightIndex)
        {
            const unsigned int vertexId = mesh->mBones[boneIdx]->mWeights[weightIndex].mVertexId;
            const float weight = mesh->mBones[boneIdx]->mWeights[weightIndex].mWeight;
            AssignBoneWeight(vertices[vertexId], boneId, weight);
        }
    }
}

unsigned int Model::TextureFromFile(const char* path, TextureData& outTex)
{
    const std::string filename = path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        const GLenum format = nrComponents == 1 ? GL_RED :
                              nrComponents == 3 ? GL_RGB : GL_RGBA;
        bool hasAlpha = format == GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (int)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);

        outTex.ID = textureID;
        outTex.Type = "texture_diffuse";
        outTex.Path = filename;
        outTex.HasAlpha = hasAlpha;
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        outTex.HasAlpha = false;
    }

    return textureID;
}

std::vector<TextureData> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    std::vector<TextureData> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for(auto & m_LoadedTexture : m_LoadedTextures)
        {
            if(std::strcmp(m_LoadedTexture.Path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_LoadedTexture);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            TextureData texture;
            texture.Type = typeName;
            texture.Path = str.C_Str();
            TextureFromFile(str.C_Str(), texture);
            textures.push_back(texture);
            m_LoadedTextures.push_back(texture);
        }
    }
    return textures;
}
