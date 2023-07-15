cmake_minimum_required(VERSION 3.0.0)
project(NoiseShaderSandbox VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB Sources source/*.cpp)
add_executable(NoiseShaderSandbox ${Sources})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(NoiseShaderSandbox PRIVATE glfw)

find_package(glbinding CONFIG REQUIRED)
target_link_libraries(NoiseShaderSandbox PRIVATE glbinding::glbinding glbinding::glbinding-aux)

find_package(efsw CONFIG REQUIRED)
target_link_libraries(NoiseShaderSandbox PRIVATE efsw::efsw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(NoiseShaderSandbox PRIVATE imgui::imgui)

find_package(OpenGL REQUIRED)
target_link_libraries(NoiseShaderSandbox PRIVATE ${OPENGL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
