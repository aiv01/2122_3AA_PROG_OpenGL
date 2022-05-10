#pragma once

typedef unsigned int GLuint;

class Ex02QuadDraw
{
public:
    void Start();
    void Update();
    void Destroy();
private:
    GLuint ProgramId;
    GLuint Vao;
    GLuint Vbo;
};