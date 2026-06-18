#include "shape.hpp"
using namespace objects::shapes;

ShapeData::ShapeData(int type, std::array<float, 8> data) : m_type(type) {
    std::copy(data.begin(), data.end(), std::begin(m_data));
}
