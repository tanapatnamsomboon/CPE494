#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    InvalidateView();
}

void Camera::SetOrientation(const glm::vec3& forward, const glm::vec3& upWorld)
{
    m_Forward = glm::normalize(forward);
    m_Right = glm::normalize(glm::cross(m_Forward, upWorld));
    m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    InvalidateView();
}

void Camera::SetAspect(float aspect)
{
    m_Aspect = aspect;
    InvalidateProjection();
}

void Camera::SetNearFar(float near, float far)
{
    m_Near = near;
    m_Far = far;
    InvalidateProjection();
}

void Camera::SetVerticalFovRadians(float verticalFov)
{
    m_VerticalFov = verticalFov;
}

void Camera::SetOrthoHalfExtents(const glm::vec2& halfExtents)
{
    m_OrthoHalfExtents = halfExtents;
    InvalidateProjection();
}

void Camera::SetYawPitch(float yawDeg, float pitchDeg)
{
    m_Yaw = yawDeg;
    m_Pitch = glm::clamp(pitchDeg, -89.9f, 89.9f);
}

const glm::mat4& Camera::GetViewMatrix()
{
    if (m_ViewDirty) RecomputeView();
    return m_ViewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix()
{
    if (m_ProjectionDirty) RecomputeProjection();
    return m_ProjectionMatrix;
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::ScreenPointToRayNdc(const glm::vec2& ndc, glm::vec3& outOrigin, glm::vec3& outDir)
{
    const glm::mat4 invVP = glm::inverse(GetViewProjectionMatrix());
    glm::vec4 pNear = invVP * glm::vec4(ndc.x, ndc.y, -1.0f, 1.0f);
    glm::vec4 pFar  = invVP * glm::vec4(ndc.x, ndc.y,  1.0f, 1.0f);
    pNear /= pNear.w;
    pFar  /= pFar.w;
    outOrigin = glm::vec3(pNear);
    outDir = glm::normalize(glm::vec3(pFar - pNear));
}

Camera::Camera(ProjectionType type)
    : m_ProjectionType(type)
{
}

void Camera::RecomputeView()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
    m_ViewDirty = false;
}

void Camera::RecomputeProjection()
{
    if (m_ProjectionType == ProjectionType::Perspective)
    {
        m_ProjectionMatrix = glm::perspective(m_VerticalFov, m_Aspect, m_Near, m_Far);
    }
    else
    {
        float lx = m_OrthoHalfExtents.x;
        float ly = m_OrthoHalfExtents.y;
        m_ProjectionMatrix = glm::ortho(-lx, lx, -ly, ly, m_Near, m_Far);
    }
    m_ProjectionDirty = false;
}
