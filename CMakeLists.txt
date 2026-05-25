set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.5.0)
project(raymarcher VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

add_executable("${CMAKE_PROJECT_NAME}"
    src/main.cpp src/glad.c
    src/screen.cpp src/screen.hpp
    src/stb_image.cpp src/stb_image.h
    src/util.cpp src/util.hpp
    src/shader/shader.cpp src/shader/shader.hpp
    src/shader/shader_builder.cpp src/shader/shader_builder.hpp
    src/bindable.hpp
    src/camera/camera.cpp src/camera/camera.hpp
    src/camera/center_camera.cpp src/camera/center_camera.hpp
)

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE include)

target_link_libraries("${CMAKE_PROJECT_NAME}" glfw OpenGL::GL)
