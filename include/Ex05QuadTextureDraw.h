#pragma once
#include "Common.h"

class Ex05QuadTextureDraw
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
    OGLTexture* SmileOGLTexture;
    OGLTexture* WoodOGLTexture;

    float ElapsedTime;
};