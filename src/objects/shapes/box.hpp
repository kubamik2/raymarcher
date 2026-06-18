#pragma once
#include "shape.hpp"
namespace objects::shapes {
class Box : public Shape {
public:
    ShapeData data() override;
    glm::vec3 m_size;
    const char* name() override {
        return "Box";
    }
private:
};
}
