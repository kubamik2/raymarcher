#pragma once
#include <glm/glm.hpp>
#include "../bindable.hpp"
#include "../components/components.hpp"
#include "game_object.hpp"

namespace objects {
class Camera : public GameObject, public Bindable {
public:
    Camera() {}; 
    void bind(GLuint program) override;
    glm::vec3 get_direction();
};
}
