#pragma once

#include "camera.h"

class FollowCamera final : public Camera
{
public:
    FollowCamera()
        : Camera(ProjectionType::Perspective)
    {
    }

    void SetTarget(const glm::vec3& target) { m_Target = target; }

    void SetDistance(float distance);
    void SetHeight(float height);
    void SetSmoothing(float smoothing);
    void SetPitchLimits(float minDeg, float maxDeg);

    void AddYawPitch(float yawDeg, float pitchDeg);
    void UpdateFollow(float dt);

    [[nodiscard]] float GetYaw() const { return m_Yaw; }
    [[nodiscard]] float GetPitch() const { return m_Pitch; }

private:
    glm::vec3 m_Target{ 0.0f };
    float m_Distance{ 8.0f };
    float m_Height{ 3.0f };
    float m_Smooth{ 0.12f };
    float m_Yaw{ 0.0f };
    float m_Pitch{ 15.0f };
    float m_MinPitch{ -44.0f };
    float m_MaxPitch{ 44.0f };
};