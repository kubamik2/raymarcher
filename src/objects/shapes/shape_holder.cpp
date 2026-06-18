#include "shape_holder.hpp"
using namespace objects::shapes;

void add_shape_data(std::vector<ShapeData> &data, objects::GameObject* object) {
    auto shape = dynamic_cast<Shape*>(object);
    data.push_back(shape->data());
    for (auto child : shape->children()) {
        add_shape_data(data, child);
    }
}

std::vector<ShapeData> ShapeHolder::recreate_shape_data() {
    std::vector<ShapeData> out{};
    for (auto child : children()) {
        add_shape_data(out, child);
    }

    return out;
}
