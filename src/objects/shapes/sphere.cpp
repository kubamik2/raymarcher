#include "sphere.hpp"

objects::shapes::ShapeData objects::shapes::Sphere::data() {
    glm::vec3 translation = global_transform().translation();
    return objects::shapes::ShapeData{0, {translation.x, translation.y, translation.z, m_radius}};
}
