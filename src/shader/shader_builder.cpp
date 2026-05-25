#include "../util.hpp"
#include "shader_builder.hpp"

ShaderBuilder& ShaderBuilder::add_module(const char* path, GLenum type) {
    this->modules.push_back(create_shader_module(path, type));
    return *this;
}

Shader ShaderBuilder::build() {
    GLuint program_id = create_program(std::span { this->modules });
    for (const GLuint shader : this->modules) {
        glDeleteShader(shader);
    }
    return Shader { program_id };
}
