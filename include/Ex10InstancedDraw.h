#pragma once
#include "Common.h"
#include <glm/glm.hpp>
#include <vector>


struct MyInstance {
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};

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
    OGLTexture* SmileOGLTexture;
    OGLTexture* WoodOGLTexture;

    float ElapsedTime;

    glm::mat4 View;
    glm::mat4 Projection;


     GLuint VboMvp;
     MyInstance Instances[10];
     std::vector<glm::mat4> MvpData;
};