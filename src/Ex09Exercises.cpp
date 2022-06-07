#pragma once
#include "Ex09Exercises.h"
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

void Ex09Exercises::Start() 
{
    /*********************/
    /* STOORMTRUP OBJECT */
    /*********************/
    Program = new OGLProgram("resources/shaders/phong.vert", 
                             "resources/shaders/phong.frag");
    
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
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    ElapsedTime = 0.f;

    //5. Create Texture
    Texture = new OGLTexture("resources/models/stormtrooper.png");
    Texture->Bind(GL_TEXTURE0);

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
    glm::vec3 PointLightPos = glm::vec3(4.f, 0.f, 0.f);
    Program->SetUniform("point_light_pos", PointLightPos);
    Program->SetUniform("camera_pos", Position);

    /***************/
    /* QUAD OBJECT */
    /***************/
    QuadProgram = new OGLProgram("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");
    std::vector<float> QuadVertices = {
       -0.2f, -0.2f, 0.0f, //bottom left
        0.2f, -0.2f, 0.0f, //bottom right
       -0.2f,  0.2f, 0.0f, //top left

        0.2f,  0.2f, 0.0f, //top right
       -0.2f,  0.2f, 0.0f, //top left
        0.2f, -0.2f, 0.0f  //bottom right
    };

    //1. Create VAO
    glGenVertexArrays(1, &QuadVao);
    glBindVertexArray(QuadVao);

    //2. Create VBO to load data
    glGenBuffers(1, &QuadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVbo);
    glBufferData(GL_ARRAY_BUFFER, QuadVertices.size() * sizeof(float), QuadVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Ex09Exercises::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //For Each Model / Program
    //1. Bind Program (and related Uniform)
    //2. Bind Texture
    //3. Bind VAO
    //4. DrawCall


    //STOORM OBJECT/PROGRAM EXECUTION
    glViewport(0, 0, 800, 600);
    Program->Bind();
    Texture->Bind(GL_TEXTURE0);
    glBindVertexArray(Vao);

    ElapsedTime += InDeltaTime;    
    float Angle = 20.f * ElapsedTime;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0.f, -4.f, 0.f));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;
    
    if (ElapsedTime < 5000.f)
    {
        // Il program quando bindato si "ricorda" i valori uniform settati in precedenza
        Program->SetUniform("mvp", Mvp);
        Program->SetUniform("model", Model);
    }
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

	glViewport(550, 400, 200, 150); //Nota si allunga se l'aspect ratio della camera non e' piu lo stesso
    /*
    glm::mat4 Model2 = glm::mat4(1.f);
    Model2 = glm::translate(Model2, glm::vec3(4.f, -4.f, 0.f));
    Model2 = glm::rotate(Model2, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
    Model2 = glm::scale(Model2, glm::vec3(2.f));
    glm::mat4 Mvp2 = Projection * View * Model2;

    Program->SetUniform("mvp", Mvp2);
    Program->SetUniform("model", Model2);
    */
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    //QUAD OBJECT/PROGRAM EXECUTION
    glViewport(0, 0, 400, 400);
    QuadProgram->Bind();
    glBindVertexArray(QuadVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}   

void Ex09Exercises::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Texture;
    delete Program;

    glDeleteVertexArrays(1, &QuadVao);
    glDeleteBuffers(1, &QuadVbo);
    delete QuadProgram;
}
