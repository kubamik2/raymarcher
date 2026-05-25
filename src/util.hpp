#pragma once
#include <span>
#include <glad/glad.h>

GLuint create_shader_module(const char *path, GLenum shader_type);
GLuint create_program(std::span<GLuint> shaders);
