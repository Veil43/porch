cmake_minimum_required(VERSION 3.20)

project(porch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE_DEBUG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(porch src/main.cc
    src/utils.cc
    src/vec3.cc
    src/color.cc
    src/ray.cc
    src/sphere.cc
    src/hittable_list.cc
    src/camera.cc
    src/material.cc
    src/aabb.cc
    src/bvh.cc
    src/texture.cc
    src/quad.cc)

# Add third-parties here
target_include_directories(porch PRIVATE third-party/stb/)

if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
    add_definitions(-DPORCH_DEBUG)
endif()