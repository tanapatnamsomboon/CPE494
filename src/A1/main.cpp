#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <Shader.h>

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

    std::vector<float> vertices;
    for (int i = -100; i <= 100; i++)
    {
        for (int j = -100; j <= 100; j++)
        {
            vertices.push_back(i / 100.0f);
            vertices.push_back(j / 100.0f);
        }
    }

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(win))
    {
        float time = glfwGetTime();

        glClear(GL_COLOR_BUFFER_BIT);
        shader.Use();
        glUniform1f(glGetUniformLocation(shader.m_ID, "uTime"), time);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, vertices.size() / 2);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
}
