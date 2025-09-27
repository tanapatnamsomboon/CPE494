#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vCode = LoadFile(vertexPath);
    std::string fCode = LoadFile(fragmentPath);

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vCode.c_str());
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fCode.c_str());

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);

    int success;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info[1024];
        glGetProgramInfoLog(m_ID, sizeof(info), nullptr, info);
        std::cerr << info << std::endl;
        exit(1);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::Use() const
{
    glUseProgram(m_ID);
}

void Shader::SetMat3(const std::string &name, const float *value) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetMat4(const std::string &name, const float *value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetVec3(const std::string &name, const float *value) const
{
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

std::string Shader::LoadFile(const char *fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Can't open file" << std::endl;
        exit(1);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint Shader::CompileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        char info[1024];
        glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
        std::cerr << info << std::endl;
        return 0;
    }
    return shader;
}
