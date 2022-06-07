#pragma once
#include "Common.h"
#include <glm/glm.hpp>

class Ex10InstancedDraw
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

    glm::mat4 View;
    glm::mat4 Projection;
};