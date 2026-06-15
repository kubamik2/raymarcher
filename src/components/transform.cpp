#include "transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

glm::mat4 components::Transform::matrix() {
    glm::mat4 t = glm::translate(glm::identity<glm::mat4>(), m_translation);
    glm::mat4 r = glm::mat4_cast(m_rotation);
    glm::mat4 s = glm::scale(glm::identity<glm::mat4>(), m_scale);

    return t * r * s;
}

components::Transform components::Transform::operator*(components::Transform &transform) {
    return components::Transform {
        matrix() * glm::vec4(transform.m_translation, 1.0f),
        m_scale * transform.m_scale,
        m_rotation * transform.m_rotation
    };
}
