#pragma once
#include "../../objects/shapes/shape.hpp"

namespace gui {
class ShapeMenu {
public:
    void render(objects::shapes::Shape *shape);
private:
    static constexpr const char* m_items[] = {
        "union",
        "subtraction",
        "intersection",
        "smooth union",
        "smooth subtraction",
        "smooth intersection"
    };
    static constexpr objects::shapes::OpType m_items_ops[] = {
        objects::shapes::OpType::UNION,
        objects::shapes::OpType::SUBTRACTION,
        objects::shapes::OpType::INTERSECTION,
        objects::shapes::OpType::SMOOTH_UNION,
        objects::shapes::OpType::SMOOTH_SUBTRACTION,
        objects::shapes::OpType::SMOOTH_INTERSECTION,
    };
    size_t m_selected;
    std::optional<size_t> m_current_shape_id;
    char m_order_buf[32];
};
}
