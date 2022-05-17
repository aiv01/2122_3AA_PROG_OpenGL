#pragma once
#include "Common.h"
class Ex01TriangleDraw
{
public:
    void Start();
    void Update();
    void Destroy();
private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
};