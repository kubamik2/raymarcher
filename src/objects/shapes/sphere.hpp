#pragma once
#include "shape.hpp"
namespace objects::shapes {
class Sphere : public Shape {
public:
    ShapeData data() override;
    float m_radius;
    const char* name() override {
        return "Sphere";
    }
private:
};
}
