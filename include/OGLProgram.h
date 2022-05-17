#pragma once
#include <string>
#include "Common.h"
class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath);
    ~OGLProgram();
    void Bind();
private:
    GLuint Id;
};