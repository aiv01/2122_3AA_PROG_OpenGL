#pragma once
#include "Ex12DeferredDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Ex12DeferredDraw::Start() 
{
    Program = new OGLProgram("resources/shaders/deferredgbuffer.vert", 
                             "resources/shaders/deferredgbuffer.frag");
    
    std::vector<float> Vertices; //position, uv, normal


    obj_mesh_t* mesh = obj_parser_parse("resources/models/stormtrooper.obj");
    
    for (int i=0; i < mesh->triangles_count; ++i)
    {
        obj_triangle_t& t = mesh->triangles[i];
        Vertices.push_back(t.v1.position.x);
        Vertices.push_back(t.v1.position.y);
        Vertices.push_back(t.v1.position.z);
        Vertices.push_back(t.v1.uv.x);
        Vertices.push_back(t.v1.uv.y);
        Vertices.push_back(t.v1.normal.x);
        Vertices.push_back(t.v1.normal.y);
        Vertices.push_back(t.v1.normal.z);

        Vertices.push_back(t.v2.position.x);
        Vertices.push_back(t.v2.position.y);
        Vertices.push_back(t.v2.position.z);
        Vertices.push_back(t.v2.uv.x);
        Vertices.push_back(t.v2.uv.y);
        Vertices.push_back(t.v2.normal.x);
        Vertices.push_back(t.v2.normal.y);
        Vertices.push_back(t.v2.normal.z);

        Vertices.push_back(t.v3.position.x);
        Vertices.push_back(t.v3.position.y);
        Vertices.push_back(t.v3.position.z);
        Vertices.push_back(t.v3.uv.x);
        Vertices.push_back(t.v3.uv.y);
        Vertices.push_back(t.v3.normal.x);
        Vertices.push_back(t.v3.normal.y);
        Vertices.push_back(t.v3.normal.z);
    }
    VerticeCount = Vertices.size() / 8;

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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    

    //4. Set Viewport
    int WinWidth = 800;
    int WinHeight = 600;

    glViewport(0, 0, WinWidth, WinHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);

    ElapsedTime = 0.f;

    //5. Create Texture
    Texture = new OGLTexture("resources/models/stormtrooper.png");
    
    //camera
    glm::vec3 Position = glm::vec3(0.f, 0.f, 8.f);
    glm::vec3 Direction = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 Up = glm::vec3(0.f, 1.f, 0.f);
    float FovY = 60.f;
    float AspectRatio = (float)WinWidth / (float)WinHeight;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    //M*V*P
    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);


    /*******************************************/
    //Prepare G-Buffer (MRT Multi-RenderTarget). In our case: Positions, Normals, Diffuse(albedo)
    /*******************************************/

    //1. Create and Bind G-Buffer (alias Geometry-Buffer) as FrameBuffer
    glGenFramebuffers(1, &GBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, GBufferId);

    //2. Create RenderTarget (as color attach): Positions (x3 float => RGB with 16bit precision)
    glGenTextures(1, &PositionTextId);
    glBindTexture(GL_TEXTURE_2D, PositionTextId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WinWidth, WinHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PositionTextId, 0);

    //3. Create RenderTarget (as color attach): Normals (x3 float => RGB with 16bit precision)
    glGenTextures(1, &NormalTextId);
    glBindTexture(GL_TEXTURE_2D, NormalTextId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WinWidth, WinHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalTextId, 0);

    //3. Create RenderTarget (as color attach): Diffuse (x3 float => RGB with 8bit precision)
    glGenTextures(1, &DiffuseTextId);
    glBindTexture(GL_TEXTURE_2D, DiffuseTextId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WinWidth, WinHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, DiffuseTextId, 0);

    //4. Set MRT to be available for the Fragment Shader
    GLenum Attachs[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, Attachs); 

    //5. Attach Depth RenderBuffer
    glGenRenderbuffers(1, &DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, DepthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WinWidth, WinHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRbo);
    
    //6. finally check if framebuffer is complete and reset to default framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) DIE("Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /*******************************************/
    //Prepare Blending Pass (before this is already possibile to visualize GBuffer)
    /*******************************************/
    
    LightProg = new OGLProgram("resources/shaders/deferredlight.vert", "resources/shaders/deferredlight.frag");

    std::vector<float> QuadVertices = {
        // positions        // texture Coords
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //bottom-left
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  //top-right
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  //top-left
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  //top-right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  //bottom-left
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  //bottom-right
    };

    // Setup Quad VAO and Buffer
    glGenVertexArrays(1, &QuadVao);
    glGenBuffers(1, &QuadVbo);
    glBindVertexArray(QuadVao);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVbo);
    glBufferData(GL_ARRAY_BUFFER, QuadVertices.size() * sizeof(float), QuadVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    LightProg->Bind();
    
    LightProg->SetUniform("cameraPos", Position);

    LightProg->SetUniform("pointLightPos[0]", glm::vec3(4.f, 0.f, 0.f));
    LightProg->SetUniform("pointLightPos[1]", glm::vec3(-4.f, 0.f, 0.f));
    LightProg->SetUniform("pointLightPos[2]", glm::vec3(0.f, 4.f, 0.f));

}

static void debug_gBuffer(GLuint GbufferId) {
    //TOP LEFT (0, 300)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, GbufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, 800, 600, //Source Bounds
                    0, 300, 400, 600, //Destin Bounds 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);

    //TOP RIGHT (400, 300)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, GbufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, 800, 600,
                    400, 300, 800, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    //BOTTOM LEFT (0, 0)
    glBindFramebuffer(GL_READ_FRAMEBUFFER, GbufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(0, 0, 800, 600,
                    0, 0, 400, 300, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Ex12DeferredDraw::Update(float InDeltaTime)
{
    ElapsedTime += InDeltaTime;
    
    float Angle = 20.f * ElapsedTime;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0.f, -4.f, 0.f));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
    Model = glm::scale(Model, glm::vec3(2.f));
    glm::mat4 Mvp = Projection * View * Model;
    

    //[GEOMETRY-PASS]
    //1. Set G-Buffer as write-buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GBufferId);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //2. Bind Program and uniform
    Program->Bind();
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    Texture->Bind(GL_TEXTURE0);
    //3. Bind Vao
    glBindVertexArray(Vao);
    //4. Draw Call
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    //[BLENDING-PASS]
    //1. Bind Main framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    // Test G-Buffer textures on the screen
    //debug_gBuffer(GBufferId);
    //return; 

    //2. Set G-Buffer as Input for the next program
    glBindFramebuffer(GL_READ_FRAMEBUFFER, GBufferId);
    
    //3. Bind program, Quad VAO and Texture (from the G-Buffer)
    LightProg->Bind();
    glBindVertexArray(QuadVao);
    glBindTextureUnit(0, PositionTextId);
    glBindTextureUnit(1, NormalTextId);
    glBindTextureUnit(2, DiffuseTextId);
    
    //4. Draw Call
    glDrawArrays(GL_TRIANGLES, 0, 6);
}   

void Ex12DeferredDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Texture;
    delete Program;

    delete LightProg;
    glDeleteVertexArrays(1, &QuadVao);
    glDeleteBuffers(1, &QuadVbo);
    glDeleteTextures(1, &PositionTextId);
    glDeleteTextures(1, &NormalTextId);
    glDeleteTextures(1, &DiffuseTextId);
    glDeleteFramebuffers(1, &GBufferId);
    glDeleteRenderbuffers(1, &DepthRbo);
}
