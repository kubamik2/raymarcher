#include "../util.hpp"
#include "shader.hpp"

Shader::Shader(GLuint program_id) : id(program_id) {}

GLuint Shader::get_id() {
    return this->id;
}

void Shader::use() {
    glUseProgram(this->id);
}
