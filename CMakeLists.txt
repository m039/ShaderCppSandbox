cmake_minimum_required(VERSION 3.0.0)
project(NoiseSahderSandbox VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)

add_executable(NoiseSahderSandbox source/main.cpp)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(NoiseSahderSandbox PRIVATE glfw)

find_package(glbinding CONFIG REQUIRED)
target_link_libraries(NoiseSahderSandbox PRIVATE glbinding::glbinding glbinding::glbinding-aux)

find_package(efsw CONFIG REQUIRED)
target_link_libraries(NoiseSahderSandbox PRIVATE efsw::efsw)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
