#include "animator.h"

Animator::Animator(Animation* animation)
{
    m_CurrentTime = 0.0f;
    m_CurrentAnimation = animation;

    m_FinalBoneMatrices.reserve(200);

    for (int i = 0; i < 200; i++)
        m_FinalBoneMatrices.emplace_back(1.0f);
}

void Animator::Update(const float dt)
{
    m_DeltaTime = dt;
    if (m_CurrentAnimation)
    {
        m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
        m_CurrentTime = std::fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
}

void Animator::Play(Animation* pAnimation)
{
    m_CurrentAnimation = pAnimation;
    m_CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData *node, const glm::mat4& parentTransform)
{
    const std::string nodeName = node->Name;
    glm::mat4 nodeTransform = node->Transformation;

    if (Bone* bone = m_CurrentAnimation->FindBone(nodeName))
    {
        bone->Update((float)m_CurrentTime);
        nodeTransform = bone->GetLocalTransform();
    }

    const glm::mat4 globalTransformation = parentTransform * nodeTransform;

    auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
    if (boneInfoMap.contains(nodeName))
    {
        const int index = boneInfoMap[nodeName].ID;
        const glm::mat4 offset = boneInfoMap[nodeName].Offset;
        m_FinalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->ChildrenCount; i++)
        CalculateBoneTransform(&node->Children[i], globalTransformation);
}
