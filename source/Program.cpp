#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "Program.hpp"
#include "FileUtils.hpp"
#include "linmath.h"

static const struct
{
    float x, y;
} vertices[6] =
    {
        {-0.5f, -0.5f},
        {-0.5f, 0.5f},
        {0.5f, 0.5f},
        {-0.5f, -0.5f},
        {0.5f, -0.5f},
        {0.5f, 0.5f}};

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
    std::filesystem::path vertexShaderPath = "../shaders/SolidColor.vs";
    std::filesystem::path fragmentShaderPath = "../shaders/SolidColor.fs";

    // Vertex shader.
    _vertexShader = CreateShader(gl::GL_VERTEX_SHADER, vertexShaderPath);

    // Fragment shader.
    _fragmentShader = CreateShader(gl::GL_FRAGMENT_SHADER, fragmentShaderPath);

    if (_fragmentShader && _vertexShader)
    {
        _program = gl::glCreateProgram();
        gl::glAttachShader(_program, _vertexShader);
        gl::glAttachShader(_program, _fragmentShader);
        gl::glLinkProgram(_program);
    }
    else
    {
        _program = 0;
    }

    _mvpLocation = gl::glGetUniformLocation(_program, "MVP");
    _vposLocation = gl::glGetAttribLocation(_program, "vPos");

    gl::glEnableVertexAttribArray(_vposLocation);
    gl::glVertexAttribPointer(_vposLocation, 2, gl::GL_FLOAT, gl::GL_FALSE,
                              sizeof(vertices[0]), (void *)0);

    PrintGLErrors();
}

gl::GLuint Program::CreateShader(gl::GLenum type, std::filesystem::path shaderPath)
{
    auto shaderText = ReadFileAsString(shaderPath);
    if (shaderText.length() <= 0)
    {
        std::cerr << "Can't find the shader's code for " << shaderPath.c_str() << "." << std::endl;
        return 0;
    }
    else
    {
        auto shader = gl::glCreateShader(type);
        const char *text = shaderText.c_str();
        gl::glShaderSource(shader, 1, &text, NULL);
        gl::glCompileShader(shader);

        gl::GLint isCompiled = 0;
        gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &isCompiled);
        if (!isCompiled)
        {
            gl::GLint maxLength = 0;
            gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<gl::GLchar> errorLog(maxLength);
            gl::glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            std::cerr << "Shader compilation error in the file " << shaderPath.filename() << std::endl;
            std::cerr << "  " << &errorLog[0] << std::endl;

            gl::glDeleteShader(shader);

            return 0;
        }

        return shader;
    }
}

void Program::Draw(int width, int height, float time)
{
    mat4x4 m, p, mvp;

    mat4x4_identity(m);
    mat4x4_ortho(p, -0.5f, 0.5f, -0.5f, 0.5f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    if (_program)
    {
        gl::glUseProgram(_program);
        gl::glUniformMatrix4fv(_mvpLocation, 1, gl::GL_FALSE, (const gl::GLfloat *)mvp);
        gl::glDrawArrays(gl::GL_TRIANGLES, 0, 6);
    }

    PrintGLErrors();
}