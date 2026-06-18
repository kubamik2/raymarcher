#pragma once

#include "objects/game_object.hpp"
#include "objects/player.hpp"
#include "objects/shapes/shape_holder.hpp"

class World {
public:
    World();

    objects::GameObject* root() {
        return &m_root;
    }

    objects::shapes::ShapeHolder* shapes() {
        return m_shapes;
    }

    objects::Player* player() {
        return m_player;
    }
private:
    objects::GameObject m_root;
    objects::shapes::ShapeHolder *m_shapes;
    objects::Player *m_player;
};
