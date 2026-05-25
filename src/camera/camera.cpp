#include <glad/glad.h>
#include "camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() : direction(0.0, 0.0, -1.0), position(0.0, 0.0, 0.0) {}

void Camera::bind(GLuint program) {
    glUniform3f(glGetUniformLocation(program, "camera_position"), this->position.x, this->position.y, this->position.z);
    glUniform3f(glGetUniformLocation(program, "camera_direction"), this->direction.x, this->direction.y, this->direction.z);
}

void Camera::rotate(float angle, glm::vec3 axis) {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, axis);
    this->direction = glm::vec3(rot * glm::vec4(this->direction, 1.0));
}

glm::vec3 Camera::getDirection() const {
    return this->direction;
}

void Camera::setDirection(glm::vec3 direction) {
    this->direction = glm::normalize(direction);
}
