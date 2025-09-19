#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    void Use() const;
    void SetMat3(const std::string &name, const float* value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetInt(const std::string &name, int value) const;

private:
    static std::string LoadFile(const char* fileName);
    static GLuint CompileShader(GLenum type, const char* source);

public:
    GLuint ID;
};