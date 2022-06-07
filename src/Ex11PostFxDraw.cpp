#pragma once
#include "Ex11PostFxDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/gtc/matrix_transform.hpp>

void Ex11PostFxDraw::Start() 
{
    Program = new OGLProgram("resources/shaders/postfx.vert", 
                             "resources/shaders/postfx.frag");
    
    std::vector<float> Vertices = {
        //FRONT FACE
        -1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,  -1.f,  1.f,   1.f, 0.f, //bottom right
        1.f,   1.f,  1.f,   1.f, 1.f, //top right
        -1.f,  1.f,  1.f,   0.f, 1.f, //top left
        -1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,   1.f,  1.f,   1.f, 1.f, //top right

        //BACK FACE
        1.f,   -1.f,  -1.f,   0.f, 0.f, //bottom left
        -1.f,  -1.f,  -1.f,   1.f, 0.f, //bottom right
        -1.f,   1.f,  -1.f,   1.f, 1.f, //top right
        1.f,    1.f,  -1.f,   0.f, 1.f, //top left
        1.f,   -1.f,  -1.f,   0.f, 0.f, //bottom left
        -1.f,   1.f,  -1.f,   1.f, 1.f, //top right

        //LEFT FACE
        -1.f, -1.f, -1.f,   0.f, 0.f, //bottom left
        -1.f, -1.f, 1.f,    1.f, 0.f, //bottom right
        -1.f,  1.f, 1.f,    1.f, 1.f, //top right
        -1.f,  1.f, -1.f,   0.f, 1.f, //top left
        -1.f, -1.f, -1.f,   0.f, 0.f, //bottom left
        -1.f,  1.f, 1.f,    1.f, 1.f, //top right

        //RIGHT FACE
        1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f, -1.f,  -1.f,  1.f, 0.f, //bottom right
        1.f, 1.f,   -1.f,  1.f, 1.f, //top right
        1.f, 1.f,   1.f,   0.f, 1.f, //top left
        1.f, -1.f,  1.f,   0.f, 0.f, //bottom left
        1.f, 1.f,   -1.f,  1.f, 1.f, //top right

        //TOP FACE
        -1.f,  1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,   1.f,  1.f,   1.f, 0.f, //bottom right
        1.f,   1.f, -1.f,   1.f, 1.f, //top right
        -1.f,  1.f, -1.f,   0.f, 1.f, //top left
        -1.f,  1.f,  1.f,   0.f, 0.f, //bottom left
        1.f,   1.f, -1.f,   1.f, 1.f, //top right

        //BOTTOM FACE
        -1.f,  -1.f,  -1.f,  0.f, 0.f, //bottom left
        1.f,   -1.f,  -1.f,  1.f, 0.f, //bottom right
        1.f,   -1.f,  1.f,   1.f, 1.f, //top right
        -1.f,  -1.f,  1.f,   0.f, 1.f, //top left
        -1.f,  -1.f,  -1.f,  0.f, 0.f, //bottom left
        1.f,   -1.f,  1.f,   1.f, 1.f //top right
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

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    ElapsedTime = 0.f;

    //5. Create Texture
    WoodOGLTexture = new OGLTexture("resources/textures/wood-box.jpg");
    WoodOGLTexture->Bind(GL_TEXTURE0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //camera
    glm::vec3 Position = glm::vec3(0.f, 0.f, 8.f);
    glm::vec3 Direction = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    //M*V*P
    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);
}

void Ex11PostFxDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ElapsedTime += InDeltaTime;
    
    float Angle = 20.f * ElapsedTime;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0.f, 0.f, 0.f));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
    Model = glm::scale(Model, glm::vec3(2.f));
    
    glm::mat4 Mvp = Projection * View * Model;
    
    Program->SetUniform("mvp", Mvp);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}   

void Ex11PostFxDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete WoodOGLTexture;
    delete Program;
}
