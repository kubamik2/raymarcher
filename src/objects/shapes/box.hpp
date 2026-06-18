#pragma once
#include "shape.hpp"
namespace objects::shapes {
class Box : public Shape {
public:
    Box(components::Transform transform, glm::vec3 size) : Shape(transform), m_size(size) {}
    ShapeData data() override;
    glm::vec3 m_size;
    const char* name() override {
        return "Box";
    }
private:
};
}
