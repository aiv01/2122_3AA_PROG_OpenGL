#pragma once
#include "Ex02QuadDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"

void Ex02QuadDraw::Start() 
{
    GLuint VertexShaderId = CreateShader("resources/shaders/triangle.vert", GL_VERTEX_SHADER);
    GLuint FragmeShaderId = CreateShader("resources/shaders/triangle.frag", GL_FRAGMENT_SHADER);
    ProgramId = CreateProgram(VertexShaderId, FragmeShaderId);

    std::vector<float> Vertices = {
        0.5f, -0.5f, 0.0f, //bottom right
       -0.5f, -0.5f, 0.0f, //bottom left
       -0.5f,  0.5f, 0.0f, //top left

       -0.5f,  0.5f, 0.0f, //top left
        0.5f,  0.5f, 0.0f, //top right
        0.5f, -0.5f, 0.0f  //bottom right
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

void Ex02QuadDraw::Update()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Ex02QuadDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteProgram(ProgramId);
}
