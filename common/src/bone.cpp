#include "bone.h"

#include "assimp_glm_helpers.h"
#include <utility>

Bone::Bone(std::string name, const int ID, const aiNodeAnim *channel)
    : m_LocalTransform(1.0f), m_Name(std::move(name)), m_ID(ID)
{
    m_NumPositions = (int)channel->mNumPositionKeys;
    for (int i = 0; i < m_NumPositions; i++)
    {
        KeyPosition data{
            AssimpGLMHelpers::GetGLMVec(channel->mPositionKeys[i].mValue),
            channel->mPositionKeys[i].mTime
        };
        m_Positions.push_back(data);
    }

    m_NumRotations = (int)channel->mNumRotationKeys;
    for (int i = 0; i < m_NumRotations; i++)
    {
        KeyRotation data{
            AssimpGLMHelpers::GetGLMQuat(channel->mRotationKeys[i].mValue),
            channel->mRotationKeys[i].mTime
        };
        m_Rotations.push_back(data);
    }

    m_NumScales = (int)channel->mNumScalingKeys;
    for (int i = 0; i < m_NumScales; i++)
    {
        KeyScale data{
            AssimpGLMHelpers::GetGLMVec(channel->mScalingKeys[i].mValue),
            channel->mScalingKeys[i].mTime
        };
        m_Scales.push_back(data);
    }
}

void Bone::Update(const float animationTime)
{
    const glm::mat4 translation = InterpolatePosition(animationTime);
    const glm::mat4 rotation    = InterpolateRotation(animationTime);
    const glm::mat4 scale       = InterpolateScale(animationTime);
    m_LocalTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(const float animationTime)
{
    for (int i = 0; i < m_NumPositions - 1; i++)
    {
        if (animationTime < m_Positions[i + 1].TimeStamp)
            return i;
    }
    assert(false);
    return 0;
}

int Bone::GetRotationIndex(const float animationTime)
{
    for (int i = 0; i < m_NumRotations - 1; i++)
    {
        if (animationTime < m_Rotations[i + 1].TimeStamp)
            return i;
    }
    assert(false);
    return 0;
}

int Bone::GetScaleIndex(const float animationTime)
{
    for (int i = 0; i < m_NumScales - 1; i++)
    {
        if (animationTime < m_Scales[i + 1].TimeStamp)
            return i;
    }
    assert(false);
    return 0;
}

double Bone::GetScaleFactor(const double lastTimeStamp, const double nextTimeStamp, const float animationTime)
{
    double scaleFactor = 0.0f;
    const double midWayLength = animationTime - lastTimeStamp;
    const double framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(const float animationTime)
{
    if (1 == m_NumPositions)
        return glm::translate(glm::mat4(1.0f), m_Positions[0].Position);

    const int p0 = GetPositionIndex(animationTime);
    const int p1 = p0 + 1;
    const double scaleFactor = GetScaleFactor(m_Positions[p0].TimeStamp, m_Positions[p1].TimeStamp, animationTime);
    const glm::vec3 finalPosition = glm::mix(m_Positions[p0].Position, m_Positions[p1].Position, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(const float animationTime)
{
    if (1 == m_NumRotations)
    {
        const auto rotation = glm::normalize(m_Rotations[0].Orientation);
        return glm::toMat4(rotation);
    }

    const int p0 = GetRotationIndex(animationTime);
    const int p1 = p0 + 1;
    const double scaleFactor = GetScaleFactor(m_Rotations[p0].TimeStamp, m_Rotations[p1].TimeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(m_Rotations[p0].Orientation, m_Rotations[p1].Orientation, (float)scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScale(const float animationTime)
{
    if (1 == m_NumScales)
        return glm::scale(glm::mat4(1.0f), m_Scales[0].Scale);

    const int p0 = GetScaleIndex(animationTime);
    const int p1 = p0 + 1;
    const double scaleFactor = GetScaleFactor(m_Scales[p0].TimeStamp, m_Scales[p1].TimeStamp, animationTime);
    const glm::vec3 finalScale = glm::mix(m_Scales[p0].Scale, m_Scales[p1].Scale, scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}
