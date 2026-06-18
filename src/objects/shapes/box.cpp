#include "box.hpp"

objects::shapes::ShapeData objects::shapes::Box::data() {
    glm::vec3 translation = global_transform().translation();
    return objects::shapes::ShapeData{1, {
        translation.x, translation.y, translation.z,
        m_size.x, m_size.y, m_size.z,
    }};
}
