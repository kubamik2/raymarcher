#pragma once
#include "../components/components.hpp"
#include <vector>
#include <optional>

namespace objects {
class GameObject {
public:
    ~GameObject() {}
    components::Transform m_transform;
    components::Transform global_transform();
private:
    std::optional<GameObject*> m_parent;
    std::vector<GameObject> m_children;
};
}
