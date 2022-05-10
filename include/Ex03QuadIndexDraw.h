#pragma once

typedef unsigned int GLuint;

class Ex03QuadIndexDraw
{
public:
    void Start();
    void Update();
    void Destroy();
private:
    GLuint ProgramId;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
};