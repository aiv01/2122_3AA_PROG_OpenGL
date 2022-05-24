#pragma once
#include <string>
#include "Common.h"


class OGLTexture
{
private:
    GLuint Id;
public:
    OGLTexture(const std::string& Path);
    ~OGLTexture();
    void Bind(GLenum TextureUnit);
};
