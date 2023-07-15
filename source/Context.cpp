#include <GLFW/glfw3.h>

#include "Context.hpp"

float Context::GetTime() {
    return (float) glfwGetTime();
}