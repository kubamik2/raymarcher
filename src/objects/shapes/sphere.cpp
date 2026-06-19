#include "sphere.hpp"

objects::shapes::ShapeData objects::shapes::Sphere::data() {
    auto transform = global_transform();
    auto mat = transform.matrix();
    return objects::shapes::ShapeData{type(), {
        mat[0][0], mat[0][1], mat[0][2], mat[0][3],
        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
        mat[3][0], mat[3][1], mat[3][2], mat[3][3],
        m_radius,
    }};
}
