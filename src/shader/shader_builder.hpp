#pragma once
#include "shader.hpp"
#include <vector>
#include <glad/glad.h>


class ShaderBuilder {
public:
    ShaderBuilder& add_module(const char* path, GLenum type);
    Shader build();
private:
    std::vector<GLuint> modules;
};
