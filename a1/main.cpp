#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"

float g_Scale = 1.0f, g_Angle = 0.0f, g_TransX = 0.0f, g_TransY = 0.0f;

void ProcessInput(GLFWwindow* win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) g_TransY += 0.01f;
    if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) g_TransY -= 0.01f;
    if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) g_TransX += 0.01f;
    if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) g_TransX -= 0.01f;
    if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) g_Scale *= 1.01f;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) g_Scale *= 0.99f;
}

int main()
{
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(800, 600, "Assignment 1", nullptr, nullptr);
    if (!win)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    Shader shader("shader.vert", "shader.frag");

    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(win))
    {
        ProcessInput(win);
        g_Angle += 0.01f;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();

        float c = cos(g_Angle), s = sin(g_Angle);
        float mat[9] = {
            g_Scale*c, -g_Scale*s, 0.0f,
            g_Scale*s,  g_Scale*c, 0.0f,
            g_TransX,   g_TransY,  1.0f,
        };
        shader.SetMat3("uTransform", mat);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
}