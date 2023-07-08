#include <filesystem>
#include <string>
#include <sstream>
#include <iostream>

#include "Program.hpp"
#include "FileUtils.hpp"
#include "linmath.h"

namespace fs = std::filesystem;

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
    {
        {-0.6f, -0.4f, 1.f, 0.f, 0.f},
        {0.6f, -0.4f, 0.f, 1.f, 0.f},
        {0.f, 0.6f, 0.f, 0.f, 1.f}};

void PrintGLErrors()
{
    gl::GLenum err;
    while ((err = gl::glGetError()) != gl::GL_NO_ERROR)
    {
        std::cerr << "Error: " << std::hex << (int)err << std::endl;
    }
}

Program::Program()
{
}

void Program::Init()
{
    // Vertex buffer.
    gl::glGenBuffers(1, &_vertexBuffer);
    gl::glBindBuffer(gl::GL_ARRAY_BUFFER, _vertexBuffer);
    gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, gl::GL_STATIC_DRAW);

    // Shaders.
    fs::path vertexShaderPath = "../shaders/SolidColor.vs";
    fs::path fragmentShaderPath = "../shaders/SolidColor.fs";

    // Vertex shader.
    auto vertexShaderText = ReadFileAsString(vertexShaderPath);
    if (vertexShaderText.length() <= 0)
    {
        std::cerr << "Can't find the vertex shader's code." << std::endl;
    }
    else
    {
        _vertexShader = gl::glCreateShader(gl::GL_VERTEX_SHADER);
        const char *text1 = vertexShaderText.c_str();
        gl::glShaderSource(_vertexShader, 1, &text1, NULL);
        gl::glCompileShader(_vertexShader);
    }

    // Fragment shader.
    auto fragmentShaderText = ReadFileAsString(fragmentShaderPath);
    if (fragmentShaderText.length() <= 0)
    {
        std::cerr << "Can't find the fragment shader's code." << std::endl;
    }
    else
    {
        _fragmentShader = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);
        const char *text2 = fragmentShaderText.c_str();
        gl::glShaderSource(_fragmentShader, 1, &text2, NULL);
        gl::glCompileShader(_fragmentShader);
    }

    _program = gl::glCreateProgram();
    gl::glAttachShader(_program, _vertexShader);
    gl::glAttachShader(_program, _fragmentShader);
    gl::glLinkProgram(_program);

    _mvpLocation = gl::glGetUniformLocation(_program, "MVP");
    _vposLocation = gl::glGetAttribLocation(_program, "vPos");
    _vcolLocation = gl::glGetAttribLocation(_program, "vCol");

    gl::glEnableVertexAttribArray(_vposLocation);
    gl::glVertexAttribPointer(_vposLocation, 2, gl::GL_FLOAT, gl::GL_FALSE,
                              sizeof(vertices[0]), (void *)0);

    gl::glEnableVertexAttribArray(_vcolLocation);
    gl::glVertexAttribPointer(_vcolLocation, 3, gl::GL_FLOAT, gl::GL_FALSE,
                              sizeof(vertices[0]), (void *)(sizeof(float) * 2));

    PrintGLErrors();
}

void Program::Draw(int width, int height, float time)
{
    auto ratio = width / (float)height;

    gl::glViewport(0, 0, width, height);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    mat4x4 m, p, mvp;

    mat4x4_identity(m);
    // mat4x4_rotate_Z(m, m, time);
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    gl::glUseProgram(_program);
    gl::glUniformMatrix4fv(_mvpLocation, 1, gl::GL_FALSE, (const gl::GLfloat *)mvp);
    gl::glDrawArrays(gl::GL_TRIANGLES, 0, 3);

    PrintGLErrors();
}