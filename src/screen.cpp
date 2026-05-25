#include "util.hpp"
#include "screen.hpp"
#include "shader/shader.hpp"
#include "shader/shader_builder.hpp"
#include <GL/gl.h>
#include <GL/glext.h>

namespace {
    GLfloat VERTICES[] = {
        -1.0, -1.0, 0.0, 0.0, 0.0,
        -1.0, 1.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0, 1.0,
        1.0, -1.0, 0.0, 1.0, 0.0
    };

    GLuint INDICES[] = {
        0, 2, 1,
        0, 3, 2
    };
}

Screen::Screen(const char *vertex_shader_path, const char *fragment_shader_path) :
shader(ShaderBuilder {}
    .add_module(vertex_shader_path, GL_VERTEX_SHADER)
    .add_module(fragment_shader_path, GL_FRAGMENT_SHADER)
    .build()
) {
    // Create screen texture
    glGenTextures(1, &this->screen_texture);
    glBindTexture(GL_TEXTURE_2D, this->screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 1920, 1200);

    // Bind the texture to an image
    glBindImageTexture(0, this->screen_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


    // Generate vertex buffer object and element buffer object
    GLuint vbo, ebo;
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Bind buffers
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // Write to buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    // Configure buffer layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

Shader& Screen::get_shader() {
    return this->shader;
}

GLuint Screen::get_screen_texture() {
    return this->screen_texture;
}

void Screen::render() {
    this->shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->screen_texture);
    glUniform1i(glGetUniformLocation(this->shader.get_id(), "screen"), 0);
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
