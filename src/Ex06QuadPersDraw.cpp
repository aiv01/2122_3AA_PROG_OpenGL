#pragma once
#include "Ex06QuadPersDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"
#include "OGLTexture.h"

void Ex06QuadPersDraw::Start() 
{
    Program = new OGLProgram("resources/shaders/quadperspectivetexture.vert", 
                             "resources/shaders/quadperspectivetexture.frag");

    std::vector<float> Vertices = {
        0.5f, -0.5f, 0.0f,  1.f, 0.f, //bottom right
       -0.5f, -0.5f, 0.0f,  0.f, 0.f, //bottom left
       -0.5f,  0.5f, 0.0f,  0.f, 1.f, //top left
        0.5f,  0.5f, 0.0f,  1.f, 1.f  //top right
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

    Color Red{1.f, 0.f, 0.f, 1.f};
    Program->SetUniform("base_color",Red);
    // GLint BaseColorLocation = glGetUniformLocation(Program->Id, "base_color");
    // Color Red{0.5f, 0.5f, 0.5f, 1.f};
    // const GLfloat* RedPtr = (GLfloat*)&Red;
    // glUniform4fv(BaseColorLocation, 1, RedPtr);

    ElapsedTime = 0.f;

    //glUniform1f(glGetUniformLocation(Program->Id, "data.value1"), 1.f);
    //glUniform1f(glGetUniformLocation(Program->Id, "data.value2"), 2.f);

    //6. Create Texture
    SmileOGLTexture = new OGLTexture("resources/textures/smile.png");
    WoodOGLTexture = new OGLTexture("resources/textures/wood-box.jpg");
    
    SmileOGLTexture->Bind(GL_TEXTURE0);
    WoodOGLTexture->Bind(GL_TEXTURE1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glUniform1i(glGetUniformLocation(Program->Id, "smile_tex"), 0);
    //glUniform1i(glGetUniformLocation(Program->Id, "wood_tex"), 1);
}

void Ex06QuadPersDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

/*
    ElapsedTime += InDeltaTime;

    Color TimeColor;
    TimeColor.R = sinf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.G = cosf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5f;
    TimeColor.A = 1.f;
    glUniform4fv(glGetUniformLocation(Program->Id, "base_color"), 1, (GLfloat*)&TimeColor);
 */
}   

void Ex06QuadPersDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    //glDeleteTextures(1, &SmileTextureId);
    delete SmileOGLTexture;
    delete WoodOGLTexture;
    delete Program;
}
