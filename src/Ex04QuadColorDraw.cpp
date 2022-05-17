#pragma once
#include "Ex04QuadColorDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"

void Ex04QuadColorDraw::Start() 
{
    Program = new OGLProgram("resources/shaders/quadcolor.vert", "resources/shaders/quadcolor.frag");

    std::vector<float> Vertices = {
        0.5f, -0.5f, 0.0f,  1.f, 0.f, 0.f, //bottom right
       -0.5f, -0.5f, 0.0f,  0.f, 1.f, 0.f,//bottom left
       -0.5f,  0.5f, 0.0f,  0.f, 0.f, 1.f,//top left
        0.5f,  0.5f, 0.0f,  1.f, 1.f, 0.f//top right
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2, //left triangle
        2, 3, 0  //right triangle
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Create EBO (element buffer)
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    int EboDataSize = Indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EboDataSize, Indexes.data(), GL_STATIC_DRAW);

    //5. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    GLint BaseColorLocation = glGetUniformLocation(Program->Id, "base_color");
    Color Red{0.5f, 0.5f, 0.5f, 1.f};
    const GLfloat* RedPtr = (GLfloat*)&Red;
    glUniform4fv(BaseColorLocation, 1, RedPtr);

    ElapsedTime = 0.f;

    //glUniform1f(glGetUniformLocation(Program->Id, "data.value1"), 1.f);
    //glUniform1f(glGetUniformLocation(Program->Id, "data.value2"), 2.f);
}

void Ex04QuadColorDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    ElapsedTime += InDeltaTime;

    Color TimeColor;
    TimeColor.R = sinf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.G = cosf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5f;
    TimeColor.A = 1.f;
    glUniform4fv(glGetUniformLocation(Program->Id, "base_color"), 1, (GLfloat*)&TimeColor);
}   

void Ex04QuadColorDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    delete Program;
}
