#pragma once
#include <string>
#include "Common.h"
class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath);
    ~OGLProgram();
    void Bind();
    void SetUniform(const std::string& InName, float InValue);
    void SetUniform(const std::string& InName, const Color& InValue);
//private:
public:
    GLuint Id;
};