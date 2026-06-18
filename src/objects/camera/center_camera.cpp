#include "center_camera.hpp"

objects::camera::CenterCamera::CenterCamera(components::Transform transform) : objects::camera::Camera(transform) {
    objects::GameObject* camera = new objects::GameObject{{}};
    camera->m_transform.translation({ 0.0f, 0.0f, 4.0f });
    m_camera = camera;
    add_child(camera);
}

void objects::camera::CenterCamera::bind(GLuint program) {
    glm::vec3 position = m_camera->global_transform().translation();
    glm::vec3 direction = this->direction();
    glUniform3f(glGetUniformLocation(program, "camera_position"), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(program, "camera_direction"), direction.x, direction.y, direction.z);
}

void objects::camera::CenterCamera::rotate_horizontal(float angle) {
    parent()->m_transform.rotate(angle, glm::vec3(0.0, 1.0, 0.0));
}

void objects::camera::CenterCamera::rotate_vertical(float angle) {
    m_transform.rotate(angle, glm::vec3(1.0, 0.0, 0.0));
    float pitch = glm::pitch(m_transform.rotation());
    if (pitch > 1.5) {
        m_transform.rotate(1.5 - pitch, glm::vec3(1.0, 0.0, 0.0));
    } else if (pitch < -1.5) {
        m_transform.rotate(-1.5 - pitch, glm::vec3(1.0, 0.0, 0.0));
    }
}

glm::vec3 objects::camera::CenterCamera::direction() {
    return m_camera->global_transform().rotation() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
}
