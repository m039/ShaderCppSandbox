#ifndef _PROGRAM_
#define _PROGRAM_

#include <glbinding/gl/gl.h>
#include <filesystem>

class Program {
public:
    Program();

    void Init();

    void Draw(int width, int height, float time);

private:
    gl::GLuint _vertexBuffer, _vertexShader, _fragmentShader, _program;

    gl::GLint _mvpLocation, _vposLocation;

    gl::GLuint CreateShader(gl::GLenum type, std::filesystem::path path);
};

#endif