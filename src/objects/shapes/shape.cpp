#include "shape.hpp"
using namespace objects::shapes;

ShapeData::ShapeData(ShapeType type, OpType op_type, std::array<float, ShapeData::DATA_SIZE> data) : m_type(type), m_op_type(op_type) {
    std::copy(data.begin(), data.end(), std::begin(m_data));
}
