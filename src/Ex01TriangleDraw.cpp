#pragma once
#include "Ex01TriangleDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>

std::string ReadFile(const std::string& InFilePath)
{
    std::ifstream InputStream(InFilePath, std::ios::ate);
    if (!InputStream.is_open()) {
        DIE("Impossible to open file");
    }

    size_t Size = InputStream.tellg();
    if (Size == -1) {
        DIE("Impossible read file size");
    }

    std::string Result;
    Result.resize(Size);

    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&Result[0], Size);

    InputStream.close();
    return Result;
}

GLuint CreateShader(const std::string& InFilePath)
{
    std::string ShaderStr = ReadFile(InFilePath);
    const char* ShaderSource = ShaderStr.c_str();
    
    GLuint ShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ShaderId, 1, &ShaderSource, NULL);
    glCompileShader(ShaderId);

    GLint Success;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {   
        GLint MaxLogLength;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        char buffer[512];
        //std::vector<GLchar> InfoLog(MaxLogLength);
        //glGetShaderInfoLog(ShaderId, GL_INFO_LOG_LENGTH, NULL, InfoLog.data());
        glGetShaderInfoLog(ShaderId, 512, NULL, buffer);
        DIE(buffer);
    }
    return ShaderId;
}


void Ex01TriangleDraw::Start() 
{
    CreateShader("resources/shaders/triangle.vert");
}

void Ex01TriangleDraw::Update()
{

}

void Ex01TriangleDraw::Destroy()
{

}
