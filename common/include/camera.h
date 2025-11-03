#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    enum class ProjectionType { Perspective, Orthographic };

    virtual ~Camera() = default;

    [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
    [[nodiscard]] const glm::vec3& GetUp() const { return m_Up; }
    [[nodiscard]] const glm::vec3& GetForward() const { return m_Forward; }
    [[nodiscard]] const glm::vec3& GetRight() const { return m_Right; }

    [[nodiscard]] float GetNearPlane() const { return m_Near; }
    [[nodiscard]] float GetFarPlane() const { return m_Far; }
    [[nodiscard]] float GetAspect() const { return m_Aspect; }

    [[nodiscard]] ProjectionType GetProjectionType() const { return m_ProjectionType; }

    virtual void SetPosition(const glm::vec3& position);
    virtual void SetOrientation(const glm::vec3& forward, const glm::vec3& upWorld);
    virtual void SetAspect(float aspect);
    virtual void SetNearFar(float near, float far);
    virtual void SetVerticalFovRadians(float verticalFov);
    virtual void SetOrthoHalfExtents(const glm::vec2& halfExtents);
    virtual void SetYawPitch(float yawDeg, float pitchDeg);

    const glm::mat4& GetViewMatrix();
    const glm::mat4& GetProjectionMatrix();
    glm::mat4 GetViewProjectionMatrix();

    virtual void ScreenPointToRayNdc(const glm::vec2& ndc, glm::vec3& outOrigin, glm::vec3& outDir);

protected:
    explicit Camera(ProjectionType type = ProjectionType::Perspective);

    void InvalidateView() { m_ViewDirty = true; }
    void InvalidateProjection() { m_ProjectionDirty = true; }

    virtual void RecomputeView();
    virtual void RecomputeProjection();

protected:
    glm::vec3 m_Position{ 0.0f, 0.0f, 3.0f };
    glm::vec3 m_Forward{ 0.0f, 0.0f, -1.0f };
    glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };

    ProjectionType m_ProjectionType{ ProjectionType::Perspective };
    float m_VerticalFov{ glm::radians(60.0f) };
    glm::vec2 m_OrthoHalfExtents{ 10.0f, 10.0f };
    float m_Aspect{ 16.0f / 9.0f };
    float m_Near{ 0.1f };
    float m_Far{ 1000.0f };

    glm::mat4 m_ViewMatrix{ 1.0f };
    glm::mat4 m_ProjectionMatrix{ 1.0f };
    bool m_ViewDirty{ true };
    bool m_ProjectionDirty{ true };

    float m_Yaw{ 0.0f };
    float m_Pitch{ 0.0f };
};