#pragma once

#include "model.h"
#include "bone.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>
#include <map>

struct AssimpNodeData
{
    glm::mat4 Transformation;
    std::string Name;
    unsigned int ChildrenCount;
    std::vector<AssimpNodeData> Children;
};

class Animation
{
public:
    Animation() = default;
    Animation(const std::string& path, Model* model);

    ~Animation() = default;

    Bone* FindBone(const std::string& name);

    [[nodiscard]] inline double GetTicksPerSecond() const { return m_TicksPerSecond; }
    [[nodiscard]] inline double GetDuration() const { return m_Duration; }
    [[nodiscard]] inline const AssimpNodeData& GetRootNode() const { return m_RootNode; }
    [[nodiscard]] inline const std::map<std::string, BoneInfo>& GetBoneIDMap() const { return m_BoneInfoMap; }

private:
    void ReadMissingBones(const aiAnimation* animation, Model& model);
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);


private:
    double m_Duration = 0.0;
    double m_TicksPerSecond = 0.0;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};