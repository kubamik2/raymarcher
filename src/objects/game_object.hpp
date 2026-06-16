#pragma once
#include "../components/components.hpp"
#include <cassert>
#include <vector>
#include <optional>

namespace objects {
class GameObject {
public:
    virtual ~GameObject();
    GameObject* parent() {
        assert(m_parent && "Game object has no parent");
        return *m_parent;
    }

    void add_child(GameObject* child);
    components::Transform global_transform();

    components::Transform m_transform;
    std::optional<GameObject*> m_parent;
    std::vector<GameObject*> m_children;
};
}
