#include "game_object.hpp"


components::Transform objects::GameObject::global_transform() {
    if (m_parent) {
        return (*m_parent)->global_transform() * m_transform;
    } else {
        return m_transform;
    }
}
