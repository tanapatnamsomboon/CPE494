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
    {;
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
