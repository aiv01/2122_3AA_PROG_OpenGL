#pragma once
#include "Common.h"
#include <glm/glm.hpp>

class Ex09Exercises
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();
private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    OGLTexture* Texture;

    glm::mat4 View;
    glm::mat4 Projection;

    int VerticeCount;

    float ElapsedTime;

    OGLProgram* QuadProgram;
    GLuint QuadVao;
    GLuint QuadVbo;
};