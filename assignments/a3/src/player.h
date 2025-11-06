#pragma once

#include <entity.h>
#include <glm/glm.hpp>
#include <model.h>
#include <animation.h>
#include <animator.h>
#include <memory>

enum class PlayerState
{
    Idle,
    Walk,
    Run
};

class Player final : public Entity
{
public:
    explicit Player(Model* model);

    void Update(float dt) override;
    void Draw(Shader& shader) override;

    void Move(const glm::vec3& dir, float dt);
    void HandleCollisions(const std::vector<std::shared_ptr<Entity>>& entities);

    [[nodiscard]] float GetYaw() const { return m_Yaw; }
    void SetYaw(float yawDeg) { m_Yaw = yawDeg; }

private:
    void ChangeState(PlayerState state);
    void ResolveCollision(const glm::vec3& otherPos, float otherRadius);

private:
    Model* m_Model;
    float m_Speed     = 2.5f;
    float m_Radius    = 0.6f;
    float m_Yaw       = 0.0f;
    float m_TargetYaw = 0.0f;
    float m_TurnSpeed = 8.0f;
    bool  m_IsMoving  = false;

    PlayerState m_State = PlayerState::Idle;
    std::unique_ptr<Animator>  m_Animator;
    std::unique_ptr<Animation> m_IdleAnimation;
    std::unique_ptr<Animation> m_WalkAnimation;
    std::unique_ptr<Animation> m_RunAnimation;
};