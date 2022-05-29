#pragma once
#include "Common.h"

class Ex07CubePersDraw
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();
private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    OGLTexture* WoodOGLTexture;

    float ElapsedTime;
};