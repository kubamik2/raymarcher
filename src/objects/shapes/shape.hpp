#pragma once
#include "../game_object.hpp"
#include <cstring>

namespace objects::shapes {
class ShapeData {
public:
    static const size_t DATA_SIZE = 24;
    ShapeData(int type, std::array<float, DATA_SIZE> data);
private:
    int m_type;
    float _pad[3];
    float m_data[DATA_SIZE];
};

class Shape : public objects::GameObject {
public:
    Shape(components::Transform transform) : objects::GameObject(transform) {}
    virtual ShapeData data() = 0;
    virtual const char* name() override {
        return "Shape";
    }
};
}
