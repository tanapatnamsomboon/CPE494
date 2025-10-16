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
        // pos         // color
       -0.5f,-0.5f,0,  1,0,0,
        0.5f,-0.5f,0,  0,1,0,
        0.5f, 0.5f,0,  0,0,1,
        0.5f, 0.5f,0,  0,0,1,
       -0.5f, 0.5f,0,  1,1,0,
       -0.5f,-0.5f,0,  1,0,0,
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