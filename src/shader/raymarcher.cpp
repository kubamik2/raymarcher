#include "raymarcher.hpp"
#include "shader_builder.hpp"
#include <cstdio>
#include <vector>

shader::Raymarcher::Raymarcher() :
    m_screen( "src/shaders/vertex.glsl", "src/shaders/fragment.glsl" ),
    m_compute_shader(ShaderBuilder{}.add_module("src/shaders/compute.glsl", GL_COMPUTE_SHADER).build())
{
    glGenBuffers(1, &m_shapes_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shapes_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_COPY);
}

void shader::Raymarcher::render(World& world, int window_width, int window_height) {
    // execute raymarching compute shader
    {
        m_compute_shader.use();
        world.player()->m_camera->bind(m_compute_shader.get_id());
        this->update_shapes_ssbo(world.shapes()->recreate_shape_data());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_screen.get_screen_texture());
        glUniform1i(glGetUniformLocation(m_compute_shader.get_id(), "screen"), 0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_shapes_ssbo);
        glDispatchCompute((window_width + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE, (window_height + WORK_GROUP_SIZE) / WORK_GROUP_SIZE, 1);
    }

    // wait for compute shader
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // draw results
    {
        m_screen.render();
    }
}

void shader::Raymarcher::update_shapes_ssbo(std::vector<objects::shapes::ShapeData> shape_data) {
    size_t size = shape_data.size() * sizeof(objects::shapes::ShapeData);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shapes_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, shape_data.data(), GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
