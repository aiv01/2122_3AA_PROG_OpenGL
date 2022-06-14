#pragma once
#include "Common.h"
#include <glm/glm.hpp>

class Ex11PostFxDraw
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

    OGLProgram* QuadProgram;
    GLuint QuadVao;
    GLuint QuadVbo;

    GLuint SceneFbo;
    GLuint SceneTex;
    GLuint SceneRbo;
};