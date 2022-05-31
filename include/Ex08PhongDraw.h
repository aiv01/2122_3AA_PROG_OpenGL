#pragma once
#include "Common.h"
#include <glm/glm.hpp>

class Ex08PhongDraw
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
    OGLTexture* Texture;

    glm::mat4 View;
    glm::mat4 Projection;

    int VerticeCount;

    float ElapsedTime;
};