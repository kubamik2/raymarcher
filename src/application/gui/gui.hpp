#pragma once
#include "../../world.hpp"
#include "shape_menu.hpp"
#include "shape_tree.hpp"

namespace gui {
class Gui {
public:
    void render(World &world);
private:
    ShapeTree m_shape_tree;
    ShapeMenu m_shape_menu;
};
}
