#include "follow_camera.h"

void FollowCamera::SetDistance(float distance)
{
    m_Distance = glm::max(0.1f, distance);
}

void FollowCamera::SetHeight(float height)
{
    m_Height = height;
}

void FollowCamera::SetSmoothing(float smoothing)
{
    m_Smooth = glm::clamp(smoothing, 0.0f, 1.0f);
}

void FollowCamera::SetPitchLimits(float minDeg, float maxDeg)
{
    m_MinPitch = glm::min(minDeg, maxDeg);
    m_MaxPitch = glm::max(minDeg, maxDeg);
    m_Pitch = glm::clamp(m_Pitch, m_MinPitch, m_MaxPitch);
}

void FollowCamera::AddYawPitch(float yawDeg, float pitchDeg)
{
    m_Yaw -= yawDeg;
    m_Pitch = glm::clamp(m_Pitch + pitchDeg, m_MinPitch, m_MaxPitch);
    InvalidateView();
}

void FollowCamera::UpdateFollow(float dt)
{
    const float cy = glm::cos(glm::radians(m_Yaw));
    const float sy = glm::sin(glm::radians(m_Yaw));
    const float cp = glm::cos(glm::radians(m_Pitch));
    const float sp = glm::sin(glm::radians(m_Pitch));

    const glm::vec3 forward = glm::normalize(glm::vec3(sy * cp, sp, cy * cp));
    const glm::vec3 right   = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
    const glm::vec3 up      = glm::normalize(glm::cross(right, forward));

    const glm::vec3 pos = m_Target - forward * m_Distance + up * m_Height;

    float alpha = 1.0f - glm::pow(1.0f - m_Smooth, glm::max(dt, 0.0f) * 60.0f);
    SetPosition(glm::mix(GetPosition(), pos, alpha));

    SetOrientation(glm::normalize(m_Target - GetPosition()), glm::vec3(0, 1, 0));
}
