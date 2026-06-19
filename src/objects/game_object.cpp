#include "game_object.hpp"
#include <algorithm>
#include <cstdio>

objects::GameObject::~GameObject() {
    for (auto object : m_children) {
        delete object;
    }
}

void objects::GameObject::add_child(objects::GameObject* child) {
    child->m_parent = this;
    m_children.push_back(child);
}

void objects::GameObject::remove_child(objects::GameObject* child) {
    // do a swap remove
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it == m_children.end()) {
        return;
    }
    std::iter_swap(it, --m_children.end());
    delete m_children.back();
    m_children.pop_back();
}

components::Transform objects::GameObject::global_transform() {
    if (m_parent) {
        return (*m_parent)->global_transform() * m_transform;
    } else {
        return m_transform;
    }
}

size_t objects::GameObject::IDS = 0;

std::optional<objects::GameObject*> objects::GameObject::find_by_id(size_t id) {
    if (this->id() == id) {
        return this;
    }
    for (auto const child : children()) {
        auto val = child->find_by_id(id);
        if (val.has_value()) {
            return val;
        }
    }
    return {};
}
