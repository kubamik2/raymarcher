#pragma once
#include "../../world.hpp"
#include "../../objects/shapes/shape.hpp"
#include <optional>

namespace gui {
class ShapeTree {
public:
    void render(World &world);
    std::optional<objects::shapes::Shape*> selected() const noexcept {
        return m_selected;
    }
private:
    std::optional<objects::shapes::Shape*> m_selected;
    void draw(objects::GameObject *object);
    bool m_delete_selected;
    bool m_popup_handled;
};
}
