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
    //Program->Bind();

    ElapsedTime = 0.f;

    //5. Create Texture
    WoodOGLTexture = new OGLTexture("resources/textures/wood-box.jpg");
    //WoodOGLTexture->Bind(GL_TEXTURE0);

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


    /************************/
    /*********NEW************/
    QuadProgram = new OGLProgram("resources/shaders/postfxquad.vert", 
                             "resources/shaders/postfxquad.frag");
    
    std::vector<float> quad = {
        -1.f, -1.f, 0.f,  0.f, 0.f, //bottom left
         1.f, -1.f, 0.f,  1.f, 0.f, //bottom right
        -1.f,  1.f, 0.f,  0.f, 1.f, //top left

        -1.f,  1.f, 0.f,  0.f, 1.f, //top left
         1.f, -1.f, 0.f,  1.f, 0.f, //bottom right
         1.f,  1.f, 0.f,  1.f, 1.f  //top right
    };

    glGenVertexArrays(1, &QuadVao);
    glBindVertexArray(QuadVao);

    glGenBuffers(1, &QuadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVbo);
    glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(float), quad.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


    int Width = 800;
    int Height = 600;

    // SCENE FRAME BUFFER
    glGenFramebuffers(1, &SceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);

    //1. Attach Color (by texture) to the framebuffer
    glGenTextures(1, &SceneTex);
    glBindTexture(GL_TEXTURE_2D, SceneTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SceneTex, 0);

    //2. Attach Depth
    glGenRenderbuffers(1, &SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, SceneRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Width, Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, SceneRbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        DIE("FrameBuffer not ready!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind and come back to SwapChain framebuffer


    //Bind Framebuffer custom
    //Progra1 -> Disegna nel FrameBuffer Custom
    //Progra2 -> Bind Texture del framebuffer
    //        -> Disegna nel Framebuffer default (o swapchaing)
}

void Ex11PostFxDraw::Update(float InDeltaTime)
{
    //1. Render Scene to Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ElapsedTime += InDeltaTime;
    float Angle = 0.f * ElapsedTime;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0.f, 0.f, 0.f));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
    Model = glm::scale(Model, glm::vec3(2.f));    
    glm::mat4 Mvp = Projection * View * Model;
    
    glBindVertexArray(Vao);
    Program->Bind();
    Program->SetUniform("mvp", Mvp);
    WoodOGLTexture->Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //2. Post Fx
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(QuadVao);
    QuadProgram->Bind();
    QuadProgram->SetUniform("time", ElapsedTime);
    glBindTextureUnit(0, SceneTex);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}   

void Ex11PostFxDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete WoodOGLTexture;
    delete Program;
}
