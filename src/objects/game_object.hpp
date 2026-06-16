#pragma once
#include "../components/components.hpp"
#include <vector>
#include <optional>

namespace objects {
class GameObject {
public:
    virtual ~GameObject();

    GameObject* parent();

    bool has_parent() {
        return m_parent.has_value();
    }

    void add_child(GameObject* child);
    components::Transform global_transform();

    components::Transform m_transform;

private:
    std::optional<GameObject*> m_parent;
    std::vector<GameObject*> m_children;
};
}
