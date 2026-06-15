#include <glad/glad.h>
#include "camera.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void objects::Camera::bind(GLuint program) {
    components::Transform global_transform = this->global_transform();
    glm::vec3 position = global_transform.translation();
    glm::vec3 direction = get_direction();
    glUniform3f(glGetUniformLocation(program, "camera_position"), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(program, "camera_direction"), direction.x, direction.y, direction.z);
}

glm::vec3 objects::Camera::get_direction() {
    return this->global_transform().rotation() * glm::vec4 { 0.0f, 0.0f, -1.0f, 1.0f };
}
