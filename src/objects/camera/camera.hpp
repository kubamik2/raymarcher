#pragma once
#include <glm/glm.hpp>
#include "../../bindable.hpp"
#include "../../components/components.hpp"
#include "../game_object.hpp"

namespace objects::camera {
class Camera : public objects::GameObject, public Bindable {
public:
    virtual void bind(GLuint program) override = 0;
    virtual void rotate_horizontal(float angle) = 0;
    virtual void rotate_vertical(float angle) = 0;
    virtual glm::vec3 direction() = 0;
    virtual ~Camera() {}
    virtual const char* name() override {
        return "Camera";
    };
private:
};
}
