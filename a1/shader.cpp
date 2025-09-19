#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vCode = LoadFile(vertexPath);
    std::string fCode = LoadFile(fragmentPath);

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vCode.c_str());
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fCode.c_str());

    ID = glCreateProgram();
    glAttachShader(ID, vs);
    glAttachShader(ID, fs);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info[1024];
        glGetProgramInfoLog(ID, sizeof(info), nullptr, info);
        std::cerr << info << std::endl;
        exit(1);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::Use() const
{
    glUseProgram(ID);
}

void Shader::SetMat3(const std::string &name, const float *value) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
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
