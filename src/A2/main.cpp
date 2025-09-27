#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float timeScale = 1.0f;

auto sunColors = glm::vec3(1.0f, 1.0f, 1.0f);
float sunIntensity = 1.0f;

float earthOrbit = 0.0f;
float earthRotation = 0.0f;
float earthOrbitSpeed = 30.0f;
float earthRotationSpeed = 50.0f;

float camDistance = 30.0f;
float pitch = 0.0f, roll = 0.0f;

unsigned int sphereVAO = 0.0f;
unsigned int sphereIndexCount = 0.0f;
glm::mat4 view, projection;

Shader shader("shader.vert", "shader.frag");

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

float GetDeltaTime()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    float dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return dt;
}

void DrawSphere(glm::mat4 model, glm::vec3 color, bool emissive = false)
{
    shader.Use();

    shader.SetMat4("model", glm::value_ptr(model));
    shader.SetMat4("view", glm::value_ptr(view));
    shader.SetMat4("projection", glm::value_ptr(projection));

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = sunColors * sunIntensity;
    shader.SetVec3("lightPos", glm::value_ptr(lightPos));
    shader.SetVec3("lightColor", glm::value_ptr(lightColor));

    glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, camDistance);
    shader.SetVec3("viewPos", glm::value_ptr(viewPos));

    shader.SetVec3("objectColor", glm::value_ptr(color));
    shader.SetBool("emissive", emissive);

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ImGuiUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Controls");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EarthUpdate()
{
    earthOrbit += earthOrbitSpeed * deltaTime * timeScale;
    earthRotation += earthRotationSpeed * deltaTime * timeScale;
}

int main()
{
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    float mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    GLFWwindow* win = glfwCreateWindow(1280 * mainScale, 800 * mainScale, "Assignment 2", nullptr, nullptr);
    if (!win) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, FramebufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) std::cerr << "Failed to initialize GLAD" << std::endl;

    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");

    while (!glfwWindowShouldClose(win))
    {
        deltaTime = GetDeltaTime();

        EarthUpdate();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(
            glm::vec3(camDistance * sin(glm::radians(roll)) * cos(glm::radians(pitch)),
                          camDistance * sin(glm::radians(pitch)),
                          camDistance * cos(glm::radians(roll)) * cos(glm::radians(roll))),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
        );
        projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 100.0f);

        glm::mat4 sunModel = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
        DrawSphere(sunModel, sunColors, true);

        glm::mat4 earthModel = glm::mat4(1.0f);
        earthModel = glm::rotate(earthModel, glm::radians(earthOrbit), glm::vec3(0, 1, 0));
        earthModel = glm::translate(earthModel, glm::vec3(10.0f, 0.0f, 0.0f));
        earthModel = glm::rotate(earthModel, glm::radians(earthRotation), glm::vec3(0, 1, 0));
        earthModel = glm::scale(earthModel, glm::vec3(1.0f));
        DrawSphere(earthModel, glm::vec3(0.2f, 0.5f, 1.0f));

        ImGuiUpdate();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(win);
    glfwTerminate();
}
