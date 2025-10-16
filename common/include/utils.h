#pragma once

#include <glad/gl.h>
#include <string>
#include <iostream>

namespace Utils
{
    inline void CheckGLError(const std::string& tag = "")
    {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cerr << "[GL ERROR] " << tag << " Code: " << err << std::endl;
        }
    }
}