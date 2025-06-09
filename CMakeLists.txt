cmake_minimum_required(VERSION 3.20)

project(porch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE_DEBUG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(porch src/main.cc
    third-party/glad/src/glad.c
    src/window.cc
    src/utils.cc
    src/renderer.cc
    src/vec3.cc
    src/color.cc
    src/ray.cc
    src/sphere.cc
    src/hittable_list.cc
    src/camera.cc
    src/material.cc
    src/aabb.cc
    src/bvh.cc)

# Add third-parties here
add_subdirectory(third-party/glfw)
target_include_directories(porch PRIVATE third-party/glad/include)
target_include_directories(porch PRIVATE third-party/stb/)

find_package(OpenGL)
target_link_libraries(porch PRIVATE OpenGL::GL)
target_link_libraries(porch PRIVATE glfw)

if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
    add_definitions(-DPORCH_DEBUG)
endif()