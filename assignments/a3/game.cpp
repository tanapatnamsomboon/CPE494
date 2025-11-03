#include "game.h"

#include "entity/static_object.h"
#include "entity/ground_plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <algorithm>
#include <iostream>

Game::Game(GLFWwindow *window)
    : m_Window(window)
    , m_Shader("shaders/model.vert", "shaders/model.frag")
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window, this);

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
        auto* game = (Game*)glfwGetWindowUserPointer(w);
        if (game) game->OnMouseMove(xpos, ypos);
    });

    m_PlayerModel   = std::make_unique<Model>("assets/objects/survivor_male/survivor_male.fbx");
    m_PineTreeModel = std::make_unique<Model>("assets/objects/pine_tree.fbx");

    m_Player = std::make_shared<Player>(m_PlayerModel.get());
    m_Player->SetPosition({0, 0, 0});
    m_OpaqueEntities.push_back(m_Player);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(-40, 39);
    for (int i = 0; i < 10; i++)
    {
        const auto x = (float)dist(gen);
        const auto z = (float)dist(gen);
        auto pineTree = std::make_shared<StaticObject>(m_PineTreeModel.get(), glm::vec3{ x, 0, z });
        if (m_PineTreeModel->HasTransparency())
            m_TransparentEntities.push_back(pineTree);
        else
            m_OpaqueEntities.push_back(pineTree);
    }
    m_OpaqueEntities.push_back(std::make_shared<GroundPlane>("assets/textures/grass_ground_texture.png"));

    m_Camera = std::make_unique<FollowCamera>();
    m_Camera->SetAspect(1280.0f / 720.0f);
    m_Camera->SetNearFar(0.1f, 100.0f);
    m_Camera->SetVerticalFovRadians(glm::radians(60.0f));
    m_Camera->SetPitchLimits(-44.0f, 44.0f);
    m_Camera->SetDistance(8.0f);
    m_Camera->SetHeight(3.0f);
    m_Camera->SetSmoothing(0.12f);
}

void Game::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        const auto current = (float)glfwGetTime();
        m_DeltaTime = current - m_LastFrame;
        m_LastFrame = current;

        ProcessInput();
        OnUpdate(m_DeltaTime);
        Render();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Game::OnUpdate(const float dt)
{
    for (const auto& e : m_OpaqueEntities)
        e->Update(dt);

    for (const auto& e : m_TransparentEntities)
        e->Update(dt);

    m_Camera->SetTarget(m_Player->GetPosition());
    m_Camera->UpdateFollow(dt);
}

void Game::Render()
{
    glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.Use();

    const glm::mat4 projection = m_Camera->GetProjectionMatrix();
    const glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader.SetMat4("uProjection", projection);
    m_Shader.SetMat4("uView", view);
    m_Shader.SetVec3("uLightPos", m_LightPos);
    m_Shader.SetVec3("uLightColor", m_LightColor);
    m_Shader.SetVec3("uViewPos", m_Camera->GetPosition());

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    for (const auto& e : m_OpaqueEntities)
        e->Draw(m_Shader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    std::ranges::sort(m_TransparentEntities,
                      [&](const auto& a, const auto& b) {
                          const float da = glm::length2(m_Camera->GetPosition() - a->GetPosition());
                          const float db = glm::length2(m_Camera->GetPosition() - b->GetPosition());
                          return da > db;
                      });

    for (const auto& e : m_TransparentEntities)
        e->Draw(m_Shader);

    glDepthMask(GL_TRUE);

    RenderUI();
}

void Game::RenderUI()
{
}

void Game::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetWindowShouldClose(m_Window, true);
        return;
    }

    const glm::vec3 forward = glm::normalize(glm::vec3(m_Camera->GetForward().x, 0.0f, m_Camera->GetForward().z));
    const glm::vec3 right   = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

    glm::vec3 move{ 0.0f };
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) move += forward;
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) move -= forward;
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) move -= right;
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) move += right;
    if (glm::length2(move) > 0.0f)
        move = glm::normalize(move);

    m_Player->Move(move, m_DeltaTime);
}

void Game::OnMouseMove(double xpos, double ypos)
{
    if (m_FirstMouse)
    {
        m_LastX = (float)xpos;
        m_LastY = (float)ypos;
        m_FirstMouse = false;
        return;
    }

    float xoffset = (float)xpos - m_LastX;
    float yoffset = m_LastY - (float)ypos;
    m_LastX = (float)xpos;
    m_LastY = (float)ypos;

    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Camera->AddYawPitch(xoffset, yoffset);
}
