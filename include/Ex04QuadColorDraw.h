#pragma once
#include "Common.h"

class Ex04QuadColorDraw
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

    float ElapsedTime;
};