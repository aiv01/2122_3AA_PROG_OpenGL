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

GLuint CreateShader(const std::string& InFilePath, GLenum InShaderType)
{
    std::string ShaderStr = ReadFile(InFilePath);
    const char* ShaderSource = ShaderStr.c_str();
    
    GLuint ShaderId = glCreateShader(InShaderType);
    glShaderSource(ShaderId, 1, &ShaderSource, NULL);
    glCompileShader(ShaderId);

    GLint Success;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {   
        GLint MaxLogLength;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetShaderInfoLog(ShaderId, MaxLogLength, NULL, InfoLog.data());
        std::string LogStr(InfoLog.begin(), InfoLog.end());
        DIE(LogStr);
    }
    return ShaderId;
}

GLuint CreateProgram(GLuint InVertId, GLuint InFragId)
{
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, InVertId);
    glAttachShader(ProgramId, InFragId);
    glLinkProgram(ProgramId);

    GLint Success;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Success);
    if (!Success)
    {   
        GLint MaxLogLength;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &MaxLogLength);

        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetProgramInfoLog(ProgramId, MaxLogLength, NULL, InfoLog.data());
        std::string LogStr(InfoLog.begin(), InfoLog.end());
        DIE(LogStr);
    }

    glDeleteShader(InVertId);
    glDeleteShader(InFragId);
    return ProgramId;
}


void Ex01TriangleDraw::Start() 
{
    GLuint VertexShaderId = CreateShader("resources/shaders/triangle.vert", GL_VERTEX_SHADER);
    GLuint FragmeShaderId = CreateShader("resources/shaders/triangle.frag", GL_FRAGMENT_SHADER);
    ProgramId = CreateProgram(VertexShaderId, FragmeShaderId);

    std::vector<float> Vertices = {
        0.5f, -0.5f, 0.0f, //bottom right
       -0.5f, -0.5f, 0.0f, //bottom left
        0.0f, 0.5f,  0.0f //top
    };

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glUseProgram(ProgramId);
}

void Ex01TriangleDraw::Update()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Ex01TriangleDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteProgram(ProgramId);
}
