#pragma once
#include <string>
#include <glad/glad.h>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    void Use() const;
    void SetMat3(const std::string& name, const float* value) const;
    void SetMat4(const std::string& name, const float* value) const;
    void SetVec3(const std::string& name, const float* value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetInt(const std::string& name, int value) const;
    void SetBool(const std::string& name, bool value) const;

private:
    static std::string LoadFile(const char* fileName);
    static GLuint CompileShader(GLenum type, const char* source);

public:
    GLuint m_ID;
};
