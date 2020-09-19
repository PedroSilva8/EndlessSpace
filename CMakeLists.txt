cmake_minimum_required(VERSION 3.0.0)
project(EndlessSpace VERSION 0.1.0)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-g3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenCL REQUIRED)
find_package(PkgConfig REQUIRED) 
find_package(Vulkan REQUIRED)

pkg_check_modules(FREETYPE REQUIRED IMPORTED_TARGET freetype2)

add_subdirectory(GameEngine)
add_subdirectory(EndlessSpace)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
