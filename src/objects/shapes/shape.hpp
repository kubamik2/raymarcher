#pragma once
#include "../game_object.hpp"
#include <cstring>

namespace objects::shapes {
class ShapeData {
public:
    ShapeData(int type, std::array<float, 8> data);
private:
    static const size_t DATA_SIZE = 8;
    int m_type;
    float _pad[3];
    float m_data[DATA_SIZE];
};

class Shape : public objects::GameObject {
public:
    virtual ShapeData data() = 0;
    virtual const char* name() override {
        return "Shape";
    }
};
}
