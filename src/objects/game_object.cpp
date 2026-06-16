#include "game_object.hpp"

objects::GameObject::~GameObject() {
    for (auto object : m_children) {
        delete object;
    }
}

void objects::GameObject::add_child(objects::GameObject* child) {
    child->m_parent = this;
    m_children.push_back(child);
}

components::Transform objects::GameObject::global_transform() {
    if (m_parent) {
        return (*m_parent)->global_transform() * m_transform;
    } else {
        return m_transform;
    }
}

objects::GameObject* objects::GameObject::parent() {
    assert(m_parent && "Game object has no parent");
    return *m_parent;
}
