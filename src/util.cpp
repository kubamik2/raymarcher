#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>

GLuint create_shader_module(const char *path, GLenum shader_type) {
    std::ifstream file { path };
    if (!file.is_open()) {
        std::string message {"Unable to open shader file at " };
        message += path;
        throw std::runtime_error(message);
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string contents = oss.str();

    const char *shader_c_str = contents.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_c_str, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[1024];
        std::string message {"Shader compilation failed at " };
        message += path;
        message += ": \n";
        glGetShaderInfoLog(shader, 1024, NULL, info_log);
        message += info_log;
        throw std::runtime_error(message);
    }
    return shader;
}

GLuint create_program(std::span<GLuint> shaders) {
    GLuint program = glCreateProgram();
    for (auto shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[1024];
        std::string message {"Failed to link program:\n " };
        glGetProgramInfoLog(program, 1024, NULL, info_log);
        message += info_log;
        throw std::runtime_error(message);
    }

    return program;
}
