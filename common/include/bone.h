#pragma once

#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

struct KeyPosition
{
    glm::vec3 Position;
    double    TimeStamp;
};

struct KeyRotation
{
    glm::quat Orientation;
    double    TimeStamp;
};

struct KeyScale
{
    glm::vec3 Scale;
    double    TimeStamp;
};

class Bone
{
public:
    Bone(std::string  name, int ID, const aiNodeAnim* channel);

    void Update(float animationTime);

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);

    [[nodiscard]] glm::mat4 GetLocalTransform() const { return m_LocalTransform; }
    [[nodiscard]] std::string GetBoneName() const { return m_Name; }
    [[nodiscard]] int GetBoneID() const { return m_ID; }

private:
    static double GetScaleFactor(double lastTimeStamp, double nextTimeStamp, float animationTime);
    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScale(float animationTime);

private:
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
    std::vector<KeyScale> m_Scales;

    int m_NumPositions;
    int m_NumRotations;
    int m_NumScales;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;
};