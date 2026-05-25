#pragma once
#include "shader/shader.hpp"
#include <glad/glad.h>

class Screen {
public:
    Screen(const char *vertex_shader_path, const char *fragment_shader_path);
    GLuint get_screen_texture();
    Shader& get_shader();
    void render();
private:
    GLuint screen_texture;
    Shader shader;
    GLuint vao;
};
