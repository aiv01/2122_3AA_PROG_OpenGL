#pragma once
#include "Ex10InstancedDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/gtc/matrix_transform.hpp>

void Ex10InstancedDraw::Start() 
{
    Program = new OGLProgram("resources/shaders/instanced.vert", 
                             "resources/shaders/instanced.frag");
    
    std::vector<float> Vertices = {
        -0.5f, -0.5f, 0.0f, 0,0,  //bottom left
         0.5f, -0.5f, 0.0f, 1,0,  //bottom right
        -0.5f,  0.5f, 0.0f, 0,1,  //top left

        -0.5f,  0.5f, 0.0f, 0,1,  //top left
         0.5f, -0.5f, 0.0f, 1,0,  //bottom right
         0.5f,  0.5f, 0.0f, 1,1,  //top right
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

    GLuint Location_5 = 5;
    glVertexAttribPointer(Location_5, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_5);

    //4. Set Viewport
    glViewport(0, 0, 1000, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    ElapsedTime = 0.f;

    //5. Create Texture
    //WoodOGLTexture = new OGLTexture("resources/textures/wood-box.jpg");
    //WoodOGLTexture->Bind(GL_TEXTURE0);
    SmileOGLTexture = new OGLTexture("resources/textures/smile.png");
    WoodOGLTexture = new OGLTexture("resources/textures/wood-box.jpg");
    
    SmileOGLTexture->Bind(GL_TEXTURE0);
    WoodOGLTexture->Bind(GL_TEXTURE1);

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


    ///////////////////////////////////////////
    glm::vec3 BasePosition{-5.f, 0.f, 0.f};
    glm::vec3 BaseRotation{0.f};
    glm::vec3 BaseScale{0.5f};

    for(int Index = 0; Index < 10; Index++)
    {
        Instances[Index].Position = BasePosition;
        BasePosition += glm::vec3(1.5f, 0.f, 0.f);
        
        Instances[Index].Rotation = BaseRotation;
        BaseRotation[2] += 10.f;
        
        Instances[Index].Scale = BaseScale;
        BaseScale += glm::vec3(0.3f, 0.3f, 0.3f);
    }

    glGenBuffers(1, &VboMvp);
    glBindBuffer(GL_ARRAY_BUFFER, VboMvp);
    glBufferData(GL_ARRAY_BUFFER, 10 * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

    GLuint Location_Mvp = 1;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    MvpData.resize(10);
}

void Ex10InstancedDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ElapsedTime += InDeltaTime;
    
    for(int Index = 0; Index < MvpData.size(); Index++) 
    {
        MyInstance& EachInstance = Instances[Index];

        //float Angle = 20.f * ElapsedTime;
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, EachInstance.Position);
        Model = glm::rotate(Model, glm::radians(EachInstance.Rotation[2] * ElapsedTime), glm::vec3(0.f, 0.f, 1.f));
        Model = glm::scale(Model, EachInstance.Scale);
        
        
        glm::mat4 Mvp = Projection * View * Model;
        MvpData[Index] = Mvp;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, MvpData.size() * sizeof(glm::mat4), MvpData.data());

    
    //Program->SetUniform("mvp", Mvp);
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 10);
}   

void Ex10InstancedDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    //delete WoodOGLTexture;
    delete Program;
}
