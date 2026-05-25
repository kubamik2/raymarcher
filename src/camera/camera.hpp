#pragma once
#include <glm/glm.hpp>
#include "../bindable.hpp"

class Camera : public Bindable {
public:
    glm::vec3 position;

    Camera();
    void bind(GLuint program) override;
    void rotate(float angle, glm::vec3 axis);
    glm::vec3 getDirection() const;
    void setDirection(glm::vec3 direction);
private:
    glm::vec3 direction;
};
