#include "shape_holder.hpp"
#include "shape.hpp"
#include <algorithm>
using namespace objects::shapes;

void add_shape_data(std::vector<ShapeData> &data, objects::GameObject* object) {
    auto shape = dynamic_cast<Shape*>(object);
    data.push_back(shape->data());
    for (auto child : shape->children()) {
        add_shape_data(data, child);
    }
}

std::vector<ShapeData> ShapeHolder::recreate_shape_data() {
    std::vector<Shape*> all = all_shapes();
    std::stable_sort(all.begin(), all.end(), [](Shape* a, Shape* b){ return b->m_order > a->m_order; });

    std::vector<ShapeData> out{};
    for (auto shape : all) {
        out.push_back(shape->data());
    }

    return out;
}

void add_shape(std::vector<Shape*> &shapes, objects::GameObject* object) {
    auto shape = dynamic_cast<Shape*>(object);
    shapes.push_back(shape);
    for (auto child : shape->children()) {
        add_shape(shapes, child);
    }
}

std::vector<Shape*> ShapeHolder::all_shapes() {
    std::vector<Shape*> all{};
    for (auto child : children()) {
        add_shape(all, child);
    }
    return all;
}
