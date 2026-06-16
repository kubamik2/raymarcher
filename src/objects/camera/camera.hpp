#pragma once
#include <glm/glm.hpp>
#include "../../bindable.hpp"
#include "../../components/components.hpp"
#include "../game_object.hpp"

namespace objects::camera {
class Camera : public objects::GameObject, public Bindable {
public:
    virtual void bind(GLuint program) = 0;
    virtual void rotate_horizontal(float angle) = 0;
    virtual void rotate_vertical(float angle) = 0;
    virtual glm::vec3 direction() = 0;
    virtual ~Camera() {}
private:
};
}
