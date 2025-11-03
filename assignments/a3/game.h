#pragma once

#include "player.h"
#include "object.h"
#include "animator.h"
#include "follow_camera.h"
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
    void RenderUI();

    void ProcessInput();
    void OnMouseMove(double xpos, double ypos);

private:
    GLFWwindow* m_Window;
    Shader      m_Shader;

    std::unique_ptr<FollowCamera> m_Camera;

    bool m_FirstMouse = true;
    float m_LastX = 640.0f, m_LastY = 360.0f;
    float m_MouseSensitivity = 0.1f;

    glm::vec3 m_LightPos{ 3.0f, 6.0f, 3.0f };
    glm::vec3 m_LightColor{ 1.0f, 1.0f, 1.0f };

    float m_LastFrame{ 0.0f };
    float m_DeltaTime{ 0.0f };

    std::unique_ptr<Model> m_PlayerModel;
    std::unique_ptr<Model> m_PineTreeModel;
    std::vector<std::unique_ptr<Model>> m_RockModels;

    std::shared_ptr<Player> m_Player;
    std::vector<std::shared_ptr<Entity>> m_OpaqueEntities;
    std::vector<std::shared_ptr<Entity>> m_TransparentEntities;
};