#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <GLFW/glfw3.h>

#undef GL_COLOR_BUFFER_BIT

int main(int argc, char *argv[])
{
    glfwInit();
    auto window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);
    glbinding::initialize(glfwGetProcAddress);

    gl::glClearColor(0.5, 0.5, 0.5, 1);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}