#include <shader.h>
#include <texture.h>
#include <camera.h>
#include "terrain.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>

static Camera camera({ 0.0f, 80.0f, 200.0f });
static float cameraSpeed = 10.0f;

static bool panelActive = false;
static bool autoSun = true;
static float sunAngle = 45.0f;
static float sunSpeed = 10.0f;
static float sunDistance = 500.0f;
static glm::vec3 sunColor = { 1.0f, 0.95f, 0.8f };

static float lastX = 400, lastY = 300;
static bool firstMouse = true;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ToggleCursor(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "assignment 2", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    Shader shader("shaders/terrain.vert", "shaders/terrain.frag");
    Terrain terrain("assets/heightmaps/terrain.png", 15.0f);
    Texture grass("assets/textures/grass.jpg");

    float heightScale = 15.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (autoSun)
        {
            sunAngle += sunSpeed * deltaTime;
            if (sunAngle > 360.0f) sunAngle -= 360.0f;
        }

        float rad = glm::radians(sunAngle);
        glm::vec3 lightPos = {
            cos(rad) * sunDistance,
            sin(rad) * sunDistance,
            0.0f
        };

        ProcessInput(window);

        glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1280.0f / 720.0f, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetMat4("uProjection", projection);
        shader.SetMat4("uView", view);

        shader.SetVec3("uLightColor", sunColor);
        shader.SetVec3("uLightPos", lightPos);
        shader.SetFloat("uLightIntensity", 1.0f);
        shader.SetVec3("uViewPos", camera.Position);
        shader.SetFloat("uHeightScale", heightScale);

        grass.Bind(0);
        shader.SetInt("uTexture", 0);

        terrain.Draw();

        // --- ImGui ---
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float alpha = panelActive ? 1.0f : 0.3f;

        ImGui::SetNextWindowBgAlpha(alpha);
        ImGui::PushItemFlag(ImGuiItemFlags_NoTabStop, true);
        ImGui::Begin("Terrain Controls");
        ImGui::ColorEdit3("Sun Color", (float*)&sunColor);
        ImGui::Checkbox("Auto Sun Movement", &autoSun);
        ImGui::SliderFloat("Sun Angle", &sunAngle, 0.0f, 360.0f);
        ImGui::SliderFloat("Sun Speed (deg/sec)", &sunSpeed, 0.0f, 60.0f);
        ImGui::SliderFloat("Height Scale", &heightScale, 1.0f, 50.0f);
        ImGui::SliderFloat("Camera Speed", &cameraSpeed, 1.0f, 100.0f);
        camera.MovementSpeed = cameraSpeed;
        ImGui::Text("Press [TAB] to toggle mouse/UI mode");
        ImGui::End();
        ImGui::PopItemFlag();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!panelActive)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard('W', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard('S', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard('A', deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard('D', deltaTime);
    }

    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        if (!tabPressed)
        {
            ToggleCursor(window);
            tabPressed = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        tabPressed = false;
    }
}

void MouseCallback(GLFWwindow*, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (!panelActive)
        camera.ProcessMouse(xoffset, yoffset);
}

void ToggleCursor(GLFWwindow *window)
{
    panelActive = !panelActive;
    glfwSetInputMode(window, GLFW_CURSOR, panelActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
