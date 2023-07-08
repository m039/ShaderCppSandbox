#define GLFW_INCLUDE_NONE

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Program.hpp"

int main(int argc, char *argv[])
{
    Program *program = new Program();

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetErrorCallback([](int error, const char *description)
                         { std::cerr << "Error: " << description << std::endl; });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    auto window = glfwCreateWindow(640, 480, "Noise Shader Sandbox", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glbinding::initialize(glfwGetProcAddress);
    glfwSwapInterval(1);

    program->Init();

    gl::glClearColor(0, 0, 0, 1);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        program->Draw(width, height, (float)glfwGetTime());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    delete program;
}