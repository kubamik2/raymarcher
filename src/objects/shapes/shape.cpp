#include "shape.hpp"
using namespace objects::shapes;

ShapeData::ShapeData(ShapeType type, std::array<float, ShapeData::DATA_SIZE> data) : m_type(type) {
    std::copy(data.begin(), data.end(), std::begin(m_data));
}
