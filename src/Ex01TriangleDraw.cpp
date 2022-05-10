#pragma once
#include "Ex01TriangleDraw.h"
#include <string>
#include <fstream>
#include "Common.h"
#include <iostream>

std::string ReadFile(const std::string& InFilePath)
{
    std::ifstream InputStream(InFilePath, std::ios::ate);
    if (!InputStream.is_open()) {
        DIE("Impossible to open file");
    }

    size_t Size = InputStream.tellg();
    if (Size == -1) {
        DIE("Impossible read file size");
    }

    std::string Result;
    Result.resize(Size);

    InputStream.seekg(0, std::ios::beg);
    InputStream.read(&Result[0], Size);

    InputStream.close();
    return Result;
}

void CreateShader(const std::string& InFilePath)
{
    std::string ShaderStr = ReadFile(InFilePath);
    std::cout << ShaderStr << "\n";
}


void Ex01TriangleDraw::Start() 
{
    CreateShader("resources/shaders/triangle.vert");
}

void Ex01TriangleDraw::Update()
{

}

void Ex01TriangleDraw::Destroy()
{

}
