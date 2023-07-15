#define GLFW_INCLUDE_NONE

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    bool show_demo_window = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    program->Init();

    while (!glfwWindowShouldClose(window))
    {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();

        gl::glViewport(0, 0, width, height);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        program->Draw((float)glfwGetTime());

        gl::GLint last_program;
        gl::glGetIntegerv(gl::GL_CURRENT_PROGRAM, &last_program);
        gl::glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        gl::glUseProgram(last_program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    delete program;
}