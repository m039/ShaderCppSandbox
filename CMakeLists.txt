cmake_minimum_required(VERSION 3.5.0)
project(ShaderCppSandbox VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB Sources source/*.cpp source/*.hpp source/*.h)
add_executable(ShaderCppSandbox ${Sources})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ShaderCppSandbox PRIVATE glfw)

find_package(glbinding CONFIG REQUIRED)
target_link_libraries(ShaderCppSandbox PRIVATE glbinding::glbinding glbinding::glbinding-aux)

find_package(efsw CONFIG REQUIRED)
target_link_libraries(ShaderCppSandbox PRIVATE efsw::efsw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(ShaderCppSandbox PRIVATE imgui::imgui)

find_package(OpenGL REQUIRED)
target_link_libraries(ShaderCppSandbox PRIVATE ${OPENGL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
