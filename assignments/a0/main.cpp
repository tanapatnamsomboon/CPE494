#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <shader.h>
#include <shape.h>
#include <utils.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum ShapeType { TRIANGLE, SQUARE, CIRCLE };

static ShapeType currentShape = TRIANGLE;
static float posX = 0.0f, posY = 0.0f;
static float angle = 0.0f;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

void ProcessInput(GLFWwindow *window);

int main()
{
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Assignment 0 - Shapes", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    unsigned int vaoTriangle = Shape::CreateTriangle();
    unsigned int vaoSquare = Shape::CreateSquare();
    unsigned int vaoCircle = Shape::CreateCircle(40);

    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posX, posY, 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0, 0, 1));
        shader.SetMat4("uTransform", model);

        switch (currentShape)
        {
            case SQUARE:
                glBindVertexArray(vaoSquare);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                break;
            case CIRCLE:
                glBindVertexArray(vaoCircle);
                glDrawArrays(GL_TRIANGLES, 0, 3 * 40); // 40 triangles per circle
                break;
            default:
                glBindVertexArray(vaoTriangle);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow *, int key, int, int action, int)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_SPACE)
            currentShape = static_cast<ShapeType>((currentShape + 1) % 3);
        if (key == GLFW_KEY_LEFT) posX -= 0.1f;
        if (key == GLFW_KEY_RIGHT) posX += 0.1f;
        if (key == GLFW_KEY_UP) posY += 0.1f;
        if (key == GLFW_KEY_DOWN) posY -= 0.1f;
        if (key == GLFW_KEY_R) angle += glm::radians(10.0f);
    }
}

void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void FramebufferSizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}
