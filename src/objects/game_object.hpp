#pragma once
#include "../components/components.hpp"
#include <cstdio>
#include <vector>
#include <optional>

namespace objects {
class GameObject {
public:
    GameObject(components::Transform transform = {}) : m_id(IDS++), m_transform(transform) {}
    virtual ~GameObject();
    virtual const char* name() {
        return "GameObject";
    }

    GameObject* parent() {
        return m_parent.value();
    }

    bool has_parent() {
        return m_parent.has_value();
    }

    void add_child(GameObject* child);
    void remove_child(GameObject* child);
    const std::vector<GameObject*>& children() const noexcept {
        return m_children;
    }

    size_t id() const noexcept {
        return m_id;
    }

    components::Transform global_transform();

    components::Transform m_transform;

private:
    static size_t IDS;
    std::optional<GameObject*> m_parent;
    std::vector<GameObject*> m_children;
    size_t m_id;
};
}
