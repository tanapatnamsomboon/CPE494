#include "shape.h"

#include <glad/gl.h>
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <vector>

unsigned int Shape::CreateTriangle() {
    float vertices[] = {
        // pos        // color
        0.0f,  0.5f, 0.0f,  1,0,0,
       -0.5f, -0.5f, 0.0f,  0,1,0,
        0.5f, -0.5f, 0.0f,  0,0,1,
    };
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return vao;
}

unsigned int Shape::CreateSquare() {
    float vertices[] = {
        // position         // color
       -0.5f, -0.5f, 0.0f,  1, 0, 0,
        0.5f, -0.5f, 0.0f,  0, 1, 0,
        0.5f,  0.5f, 0.0f,  0, 0, 1,
        0.5f,  0.5f, 0.0f,  0, 0, 1,
       -0.5f,  0.5f, 0.0f,  1, 1, 0,
       -0.5f, -0.5f, 0.0f,  1, 0, 0,
    };
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return vao;
}

unsigned int Shape::CreateCircle(int segments) {
    std::vector<float> vertices;
    for (int i = 0; i < segments; i++) {
        float theta1 = (float)i / segments * 2.0f * M_PI;
        float theta2 = (float)(i + 1) / segments * 2.0f * M_PI;
        vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 1,1,1 });
        vertices.insert(vertices.end(), { cos(theta1)*0.5f, sin(theta1)*0.5f, 0.0f, 1,0,0 });
        vertices.insert(vertices.end(), { cos(theta2)*0.5f, sin(theta2)*0.5f, 0.0f, 0,0,1 });
    }
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return vao;
}

unsigned int Shape::CreatePlane()
{
    constexpr float vertices[] = {
        // positions           // normals         // texcoords
        -50.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,
         50.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,  50.0f, 50.0f,
         50.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
        -50.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    };

    const unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VAO;
}
