#pragma once
#include "../game_object.hpp"
#include <cstring>

namespace objects::shapes {
enum ShapeType {
    SPHERE = 0,
    BOX = 1,
    FRACTAL = 2
};

enum OpType {
    UNION = 0,
    SUBTRACTION = 1,
    INTERSECTION = 2,
    SMOOTH_UNION = 3,
    SMOOTH_SUBTRACTION = 4,
    SMOOTH_INTERSECTION = 5,
};

class ShapeData {
public:
    static const size_t DATA_SIZE = 24;
    ShapeData(ShapeType type, OpType op_type, std::array<float, DATA_SIZE> data);
private:
    int m_type;
    int m_op_type;
    float _pad[2];
    float m_data[DATA_SIZE];
};

class Shape : public objects::GameObject {
public:
    Shape(components::Transform transform) : objects::GameObject(transform) {}
    virtual ShapeData data() = 0;
    virtual ShapeType type() const noexcept = 0;
    virtual const char* name() override {
        return "Shape";
    }

    OpType m_op = OpType::UNION;
    int m_order = 0;
};
}
