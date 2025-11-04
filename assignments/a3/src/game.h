#pragma once

#include "player.h"
#include "object.h"
#include "animator.h"
#include "follow_camera.h"
#include "systems/item_database.h"
#include "systems/inventory.h"
#include "entity/item_pickup.h"
#include <model.h>
#include <texture.h>
#include <shader.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Game
{
public:
    explicit Game(GLFWwindow* window);

    void Run();

private:
    void OnUpdate(float dt);
    void Render();
    void RenderSun();
    void RenderScene();
    void RenderUI();

    void ProcessInput();
    void OnMouseMove(double xpos, double ypos);

    static bool IntersectRaySphere(const glm::vec3& ro, const glm::vec3& rd, const glm::vec3& c, float r, float& tHit);

    void UpdateSunLight(float time);

    void InitShadowMap();
    void RenderShadowPass();

private:
    GLFWwindow* m_Window;

    std::unique_ptr<FollowCamera> m_Camera;

    bool m_FirstMouse = true;
    float m_LastX = 640.0f, m_LastY = 360.0f;
    float m_MouseSensitivity = 0.1f;

    float m_LastFrame{ 0.0f };
    float m_DeltaTime{ 0.0f };

    std::shared_ptr<Player> m_Player;
    std::vector<std::shared_ptr<Entity>> m_OpaqueEntities;
    std::vector<std::shared_ptr<Entity>> m_TransparentEntities;

    Inventory m_Inventory{ 16 };
    std::vector<std::shared_ptr<ItemPickup>> m_Pickups;

    glm::vec3 m_LightDir{ -0.6f, -1.0f, -0.4f };
    glm::vec3 m_LightPos{ 3.0f, 10.0f, 3.0f };
    glm::vec3 m_LightColor{ 1.0f, 1.0f, 1.0f };
    glm::vec3 m_AmbientColor{ 0.25f, 0.25f, 0.3f };

    GLuint m_SunVAO = 0, m_SunVBO = 0;

    glm::vec3 m_SkyColor{};
    glm::vec3 m_SkyColorDay{ 0.53f, 0.81f, 0.92f };
    glm::vec3 m_SkyColorNight{ 0.02f, 0.02f, 0.05f };

    unsigned int m_ShadowFBO{ 0 };
    unsigned int m_ShadowTex{ 0 };
    int m_ShadowW{ 2048 };
    int m_ShadowH{ 2048 };

    Shader m_Shader;
    Shader m_DepthShader;
    Shader m_SunShader;

    glm::mat4 m_LightView{ 1.0f };
    glm::mat4 m_LightProj{ 1.0f };
    glm::mat4 m_LightVP{ 1.0f };
    float     m_ShadowBias{ 0.002f };
};