#pragma once
#include "../game_object.hpp"
#include "shape.hpp"
#include <vector>

namespace objects::shapes {
class ShapeHolder : public GameObject {
public:
    std::vector<objects::shapes::ShapeData> recreate_shape_data();

    const char* name() override {
        return "ShapeHolder";
    }

    bool was_modified() const noexcept {
        return m_was_modified;
    }

    void clear_was_modified() {
        m_was_modified = false;
    }

private:
    bool m_was_modified;
};
}
