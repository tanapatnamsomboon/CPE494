#include "game.h"

#include "object.h"
#include "entity/static_object.h"
#include "entity/ground_plane.h"
#include "systems/item_spawner.h"
#include "systems/world_spawner.h"
#include "systems/item_database.h"
#include <model.h>
#include <texture.h>
#include <model_manager.h>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <algorithm>
#include <iostream>

static void CreateSunBillboard(GLuint& vao, GLuint& vbo, GLuint& ebo)
{
    constexpr float sunQuad[] = {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
    };
    const GLuint idx[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sunQuad), sunQuad, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Game::Game(GLFWwindow *window)
    : m_Window(window)
    , m_Shader("shaders/model.vert", "shaders/model.frag")
    , m_DepthShader("shaders/depth.vert", "shaders/depth.frag")
    , m_SunShader("shaders/sun.vert", "shaders/sun.frag")
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

    glfwSwapInterval(1);

    ItemDatabase::Init();

    const auto playerModel = ModelManager::GetModel("assets/objects/survivor_male/survivor_male.fbx");
    const auto bottleModel = ModelManager::GetModel("assets/objects/bottle_0.fbx");
    const auto pineTreeModel = ModelManager::GetModel("assets/objects/pine_tree.fbx");
    const auto rockModel = ModelManager::GetModel("assets/objects/rock_0.fbx");

    m_Player = std::make_shared<Player>(playerModel.get());
    m_Player->SetPosition({0, 0, 0});
    m_OpaqueEntities.push_back(m_Player);

    auto spawned = ItemSpawner::SpawnRandomItems(20, { 40, 40 }, 0.0f);
    for (auto& obj : spawned)
    {
        m_Pickups.push_back(obj);
        m_OpaqueEntities.push_back(obj);
    }

    auto environmentObjects = WorldSpawner::SpawnEnvironment(20, 15, 30, { 50, 50 });
    for (auto& obj : environmentObjects)
    {
        m_OpaqueEntities.push_back(obj);
    }

    m_Plane = std::make_unique<GroundPlane>("assets/textures/grass_ground_texture.png");

    m_Camera = std::make_unique<FollowCamera>();
    m_Camera->SetAspect(1280.0f / 720.0f);
    m_Camera->SetNearFar(0.1f, 100.0f);
    m_Camera->SetVerticalFovRadians(glm::radians(60.0f));
    m_Camera->SetPitchLimits(-44.0f, 44.0f);
    m_Camera->SetDistance(5.0f);
    m_Camera->SetHeight(3.0f);
    m_Camera->SetSmoothing(0.12f);

    InitShadowMap();

    GLuint ebo = 0;
    CreateSunBillboard(m_SunVAO, m_SunVBO, ebo);
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
    UpdateSunLight((float)glfwGetTime());

    for (const auto& e : m_OpaqueEntities)
        e->Update(dt);

    for (const auto& e : m_TransparentEntities)
        e->Update(dt);

    m_Camera->SetTarget(m_Player->GetPosition());
    m_Camera->UpdateFollow(dt);

    UpdateFps(dt);

    /*
    if (m_AdaptiveShadow)
    {
        if (m_SmoothedMS > 22.0f && m_ShadowW > 2048)
        {
            RecreateShadowMap(2048, 2048);
        }
        else if (m_SmoothedMS < 14.0f && m_ShadowW < 4096)
        {
            RecreateShadowMap(4096, 4096);
        }
    }
    */
}

void Game::Render()
{
    RenderShadowPass();

    int fbW = 0, fbH = 0;
    glfwGetFramebufferSize(m_Window, &fbW, &fbH);
    glViewport(0, 0, fbW, fbH);

    glClearColor(m_SkyColor.x, m_SkyColor.y, m_SkyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSun();
    RenderScene();
    RenderUI();
}

void Game::RenderSun()
{
    m_SunShader.Use();

    const glm::mat4 view = m_Camera->GetViewMatrix();
    const glm::mat4 proj = m_Camera->GetProjectionMatrix();

    const glm::vec3 sunDir = -m_LightDir;
    const glm::vec3 sunPos = m_Camera->GetPosition() + sunDir * 100.0f;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), sunPos);
    model[0][0] = view[0][0]; model[0][1] = view[1][0]; model[0][2] = view[2][0];
    model[1][0] = view[0][1]; model[1][1] = view[1][1]; model[1][2] = view[2][1];
    model[2][0] = view[0][2]; model[2][1] = view[1][2]; model[2][2] = view[2][2];
    model = glm::scale(model, glm::vec3(20.0f));
    const glm::mat4 mvp = proj * view * model;

    m_SunShader.SetMat4("uMVP", mvp);
    m_SunShader.SetVec3("uColor", m_LightColor);
    const float daylight = glm::clamp(
        (float)sin(glm::radians(fmod(glfwGetTime(), 120.0f) / 120.0f * 360.0f + 90.0f)) * 0.5f + 0.5f,
        0.0f, 1.0f
    );
    m_SunShader.SetFloat("uIntensity", 0.8f * daylight + 0.1f);

    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(m_SunVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void Game::RenderScene()
{
    m_Shader.Use();

    const glm::mat4 projection = m_Camera->GetProjectionMatrix();
    const glm::mat4 view = m_Camera->GetViewMatrix();
    const glm::vec3 camPos = m_Camera->GetPosition();

    m_Shader.SetMat4("uProjection", projection);
    m_Shader.SetMat4("uView", view);
    m_Shader.SetVec3("uViewPos", m_Camera->GetPosition());

    m_Shader.SetVec3("uLightDir", m_LightDir);
    m_Shader.SetVec3("uLightColor", m_LightColor);
    m_Shader.SetVec3("uAmbientColor", m_AmbientColor);

    m_Shader.SetMat4("uLightVP", m_LightVP);
    m_Shader.SetFloat("uShadowBias", m_ShadowBias);
    m_Shader.SetVec2("uShadowMapSize", { m_ShadowW, m_ShadowH });

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, m_ShadowTex);
    m_Shader.SetInt("shadowMap", 5);
    m_Shader.SetVec3("uLightColor", m_LightColor);

    m_Shader.SetVec3("uFogColor", m_SkyColor);
    m_Shader.SetFloat("uFogStart", 20.0f);
    m_Shader.SetFloat("uFogEnd", 50.0f);
    m_Shader.SetFloat("uFogDensity", 0.7f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    const float maxDist2 = m_VisibilityRange * m_VisibilityRange;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_Plane->Draw(m_Shader);
    for (const auto& e : m_OpaqueEntities)
    {
        if (glm::length2(camPos - e->GetPosition()) <= maxDist2)
            e->Draw(m_Shader);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    std::vector<std::shared_ptr<Entity>> visibles;
    visibles.reserve(m_TransparentEntities.size());
    for (const auto& e : m_TransparentEntities) {
        if (glm::length2(camPos - e->GetPosition()) <= maxDist2)
            visibles.push_back(e);
    }

    std::ranges::sort(visibles,
        [&](const auto& a, const auto& b) {
            const float da = glm::length2(camPos - a->GetPosition());
            const float db = glm::length2(camPos - b->GetPosition());
            return da > db;
        });

    for (const auto& e : visibles) e->Draw(m_Shader);

    glDepthMask(GL_TRUE);
}

void Game::RenderShadowPass()
{
    const glm::vec3 center   = m_Player->GetPosition();
    const float radius = m_ShadowCoverage;

    const glm::vec3 lightPos = center - m_LightDir * 40.0f;

    m_LightView = glm::lookAt(lightPos, center, glm::vec3(0, 1, 0));
    m_LightProj = glm::ortho(-radius, radius, -radius, radius, 0.1f, 120.0f);
    m_LightVP = m_LightProj * m_LightView;

    glViewport(0, 0, m_ShadowW, m_ShadowH);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glCullFace(GL_FRONT);

    m_DepthShader.Use();
    m_DepthShader.SetMat4("uLightVP", m_LightVP);

    for (const auto& e : m_OpaqueEntities)
        e->Draw(m_DepthShader);
    for (const auto& e : m_TransparentEntities)
        e->Draw(m_DepthShader);

    glCullFace(GL_BACK);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
    {
        constexpr float pickupDistance = 2.0f;
        const glm::vec3 playerPos = m_Player->GetPosition();

        int bestIndex = -1;
        float bestDist = pickupDistance;

        for (int i = 0; i < (int)m_Pickups.size(); i++)
        {
            const float dist = glm::distance(playerPos, m_Pickups[i]->GetPosition());
            if (dist < bestDist)
            {
                bestDist = dist;
                bestIndex = i;
            }
        }

        if (bestIndex >= 0)
        {
            const auto pickup = m_Pickups[bestIndex];
            const ItemId id = pickup->GetItem();
            const int count = pickup->GetCount();

            if (m_Inventory.Add(id, count))
            {
                m_Pickups.erase(m_Pickups.begin() + bestIndex);
                std::cout << "Picked up " << ItemDatabase::Get(id).Name << " x" << count << std::endl;
            }
        }
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

bool Game::IntersectRaySphere(const glm::vec3& ro, const glm::vec3& rd, const glm::vec3& c, float r, float& tHit)
{
    const glm::vec3 oc = ro - c;
    const float b = glm::dot(oc, rd);
    const float cterm = glm::dot(oc, oc) - r * r;
    const float disc = b * b - cterm;
    if (disc < 0.0f) return false;
    const float s = sqrtf(disc);
    const float t0 = -b - s;
    const float t1 = -b + s;
    tHit = t0 > 0.0f ? t0 : t1;
    return tHit > 0.0f;
}

void Game::UpdateSunLight(float time)
{
    const float dayTime = fmod(time + 30.0f, 120.0f) / 120.0f;
    const float angle = glm::radians(dayTime * 360.0f - 90.0f);

    m_LightDir = glm::normalize(glm::vec3(cos(angle), -sin(angle), -0.4f));

    float daylight = glm::clamp(sin(angle), 0.0f, 1.0f);
    daylight = glm::pow(daylight, 0.6f);

    m_LightColor   = glm::mix(glm::vec3(0.8f, 0.4f, 0.3f), glm::vec3(1.0f, 0.95f, 0.8f), daylight);
    m_AmbientColor = glm::mix(glm::vec3(0.05f, 0.05f, 0.1f), glm::vec3(0.25f, 0.25f, 0.3f), daylight);
    m_SkyColor     = glm::mix(m_SkyColorNight, m_SkyColorDay, daylight);

    const glm::vec3 center = m_Player->GetPosition();
    m_LightPos = center - m_LightDir * 40.0f;
}

void Game::InitShadowMap()
{
    glGenTextures(1, &m_ShadowTex);
    glBindTexture(GL_TEXTURE_2D, m_ShadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_ShadowW, m_ShadowH, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    constexpr float border[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glGenFramebuffers(1, &m_ShadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Shadow FBO not complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::UpdateFps(float dt)
{
    m_FpsAccum += dt;
    m_FpsTimer += dt;
    m_FpsFrames++;

    constexpr float alpha = 0.1f;
    m_SmoothedMS = (1.0f - alpha) * m_SmoothedMS + alpha * (dt * 1000.0f);

    if (m_FpsTimer >= 0.5)
    {
        const float fps = (float)m_FpsFrames / (float)m_FpsTimer;
        const float ms  = 1000.0f / glm::max(fps, 1.0f);
        UpdateWindowTitleFPS(fps, ms);
        m_FpsFrames = 0;
        m_FpsTimer  = 0.0;
    }
}

void Game::UpdateWindowTitleFPS(float fps, float ms)
{
    char title[128];
    snprintf(title, sizeof(title), "Assignment 3 - %.0f FPS (%.2f ms)", fps, ms);
    glfwSetWindowTitle(m_Window, title);
}

void Game::RecreateShadowMap(int w, int h)
{
    if (w == m_ShadowW && h == m_ShadowH) return;
    m_ShadowW = w; m_ShadowH = h;

    if (m_ShadowTex) glDeleteTextures(1, &m_ShadowTex);
    if (m_ShadowFBO) glDeleteFramebuffers(1, &m_ShadowFBO);

    InitShadowMap();
    std::cout << "[ShadowMap] resized to " << m_ShadowW << "x" << m_ShadowH << "\n";
}
