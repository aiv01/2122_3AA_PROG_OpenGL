#pragma once

typedef unsigned int GLuint;

class Ex01TriangleDraw
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