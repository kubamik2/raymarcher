#pragma once
#include "shape.hpp"
namespace objects::shapes {
class Sphere : public Shape {
public:
    Sphere(components::Transform transform, float radius) : Shape(transform), m_radius(radius) {}
    ShapeData data() override;
    float m_radius;
    const char* name() override {
        return "Sphere";
    }
private:
};
}
