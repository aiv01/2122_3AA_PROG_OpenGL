#pragma once
#include "Common.h"

class Ex03QuadIndexDraw
{
public:
    void Start();
    void Update();
    void Destroy();
private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
};