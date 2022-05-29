#pragma once
#include "Ex07CubePersDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include "OGLProgram.h"
#include "OGLTexture.h"

void Ex07CubePersDraw::Start() 
{
    Program = new OGLProgram("resources/shaders/cubeperspective.vert", 
                             "resources/shaders/cubeperspective.frag");
    
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

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Ex07CubePersDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ElapsedTime += InDeltaTime;
    Program->SetUniform("rotation", 20.f * ElapsedTime);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}   

void Ex07CubePersDraw::Destroy()
{   
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete WoodOGLTexture;
    delete Program;
}
