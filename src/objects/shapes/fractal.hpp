#pragma once
#include "shape.hpp"
namespace objects::shapes {
class Fractal : public Shape {
public:
    Fractal(components::Transform transform) : Shape(transform) {}
    ShapeData data() override;
    const char* name() override {
        return "Fractal";
    }
    ShapeType type() const noexcept override {
        return ShapeType::FRACTAL;
    }
};
}
