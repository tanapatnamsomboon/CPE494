#include "player.h"

#include "object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Player::Player(Model* model)
    : m_Model(model)
{
    m_IdleAnimation = std::make_unique<Animation>("assets/objects/survivor_male/survivor_male_idle.fbx", m_Model);
    m_WalkAnimation = std::make_unique<Animation>("assets/objects/survivor_male/survivor_male_walk.fbx", m_Model);
    m_RunAnimation = std::make_unique<Animation>("assets/objects/survivor_male/survivor_male_run.fbx", m_Model);
    m_Animator = std::make_unique<Animator>(m_IdleAnimation.get());
}

void Player::Update(const float dt)
{
    if (m_Animator)
        m_Animator->Update(dt);
}

void Player::Draw(Shader& shader)
{
    if (m_Animator)
        shader.SetBoneMatrices(m_Animator->GetFinalBoneMatrices());
    else
        shader.SetIdentityBones();

    glm::mat4 model{ 1.0f };
    model = glm::translate(model, m_Position);
    model = glm::rotate(model, glm::radians(m_Yaw), glm::vec3(0, 1, 0));
    shader.SetMat4("uModel", model);

    shader.SetBool("uUseSkinning", true);
    m_Model->Draw(shader);
}

void Player::Move(const glm::vec3& dir, float dt)
{
    if (glm::length2(dir) > 1e-6f)
    {
        const float targetYaw = glm::degrees(atan2(dir.x, dir.z));
        m_TargetYaw = targetYaw;

        float yawDiff = m_TargetYaw - m_Yaw;
        if (yawDiff > 180.0f) yawDiff -= 360.0f;
        if (yawDiff < -180.0f) yawDiff += 360.0f;

        m_Yaw += yawDiff * glm::clamp(m_TurnSpeed * dt, 0.0f, 1.0f);

        m_Position += dir * m_Speed * dt;

        m_IsMoving = true;
        ChangeState(PlayerState::Walk);
    }
    else
    {
        m_IsMoving = false;
        ChangeState(PlayerState::Idle);
    }
}

void Player::HandleCollisions(const std::vector<std::shared_ptr<Entity>>& entities)
{
    glm::vec3 totalPush(0.0f);
    int count = 0;

    for (const auto& e : entities)
    {
        if (e.get() == this || !e->HasCollision()) continue;

        glm::vec3 diff = m_Position - e->GetPosition();
        diff.y = 0.0f;
        float dist2 = glm::length2(diff);
        float minDist = m_Radius + e->GetCollisionRadius();

        if (dist2 < minDist * minDist && dist2 > 0.0001f)
        {
            float dist = sqrtf(dist2);
            glm::vec3 pushDir = diff / dist;
            float penetration = minDist - dist;
            totalPush += pushDir * penetration;
            count++;
        }
    }

    if (count > 0)
    {
        glm::vec3 avgPush = totalPush / (float)count;
        m_Position += avgPush * 0.5f;
    }
}

void Player::ResolveCollision(const glm::vec3& otherPos, float otherRadius)
{
    glm::vec3 diff = m_Position - otherPos;
    diff.y = 0.0f;
    const float dist = glm::length(diff);
    const float minDist = otherRadius + m_Radius;

    if (dist < minDist && dist > 0.0001f)
    {
        const glm::vec3 pushDir = diff / dist;
        constexpr float correction = 0.5f;
        const float penetration = minDist - dist;
        m_Position += pushDir * penetration * correction;
    }
}


void Player::ChangeState(PlayerState state)
{
    if (m_State == state)
        return;

    m_State = state;
    if (!m_Animator) return;

    switch (m_State)
    {
        case PlayerState::Idle:
            m_Animator->Play(m_IdleAnimation.get());
            break;
        case PlayerState::Walk:
            m_Animator->Play(m_WalkAnimation.get());
            break;
        case PlayerState::Run:
            m_Animator->Play(m_RunAnimation.get());
            break;
    }
}
