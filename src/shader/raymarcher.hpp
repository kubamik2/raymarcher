#pragma once
#include "../screen.hpp"
#include "shader.hpp"
#include "../world.hpp"
#include <vector>

namespace shader {
class Raymarcher {
public:
    static constexpr GLuint WORK_GROUP_SIZE = 16;
    Raymarcher();
    void render(World& world, int window_width, int window_height);
    void update_shapes_ssbo(std::vector<objects::shapes::ShapeData> shape_data);
private:
    Shader m_compute_shader;
    Screen m_screen;
    GLuint m_shapes_ssbo;
};
}
