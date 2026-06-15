#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_operation.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"

namespace components {
class Transform {
public:
    // transform

    Transform() :
        m_translation(0.0f),
        m_scale(1.0f),
        m_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

    Transform(glm::vec3 translation, glm::vec3 scale, glm::quat rotation) :
        m_translation(translation),
        m_scale(scale),
        m_rotation(rotation) {}
    
    glm::mat4 matrix();

    Transform operator*(Transform &other);

    // translation

    [[nodiscard]] glm::vec3 translation() const noexcept {
        return m_translation;
    }

    void translation(glm::vec3 translation) {
        m_translation = translation;
    }

    void translate(glm::vec3 translation) {
        m_translation += translation;
    }

    // scale

    [[nodiscard]] glm::vec3 scaling() const noexcept {
        return m_scale;
    }

    void scaling(glm::vec3 scale) {
        m_scale = scale;
    }

    void scale(glm::vec3 scale) {
        m_scale *= scale;
    }

    // rotation
    
    glm::quat rotation() const noexcept {
        return m_rotation;
    }

    void rotate(glm::quat rotation) {
        m_rotation *= rotation;
    };

    void rotate(float angle, glm::vec3 axis) {
        m_rotation *= glm::angleAxis(angle, axis);
    }

private:
    glm::quat m_rotation;
    glm::vec3 m_translation;
    glm::vec3 m_scale;
};
}
