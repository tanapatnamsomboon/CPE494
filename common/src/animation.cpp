#include "animation.h"

#include "assimp_glm_helpers.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Animation::Animation(const std::string& path, Model *model)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    const auto animation = scene->mAnimations[0];
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    ReadHierarchyData(m_RootNode, scene->mRootNode);
    ReadMissingBones(animation, *model);
}

Bone * Animation::FindBone(const std::string& name)
{
    const auto iter = std::ranges::find_if(m_Bones,
        [&](const Bone& bone)
        {
            return bone.GetBoneName() == name;
        }
    );

    if (iter == m_Bones.end())
        return nullptr;

    return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
    const unsigned int size = animation->mNumChannels;

    auto& boneInfoMap = model.GetBoneInfoMap();
    int& boneCount = model.GetBoneCount();

    for (unsigned i = 0; i < size; i++)
    {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (!boneInfoMap.contains(boneName))
        {
            boneInfoMap[boneName].ID = boneCount;
            boneCount++;
        }
        m_Bones.emplace_back(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].ID, channel);
    }

    m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
    assert(src);

    dest.Name = src->mName.data;
    dest.Transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.ChildrenCount = src->mNumChildren;

    for (unsigned i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.Children.push_back(newData);
    }
}
