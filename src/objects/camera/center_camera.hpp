#pragma once
#include "camera.hpp"

namespace objects::camera {
class CenterCamera : public Camera {
public:
    CenterCamera();
    void bind(GLuint program) override;
    void rotate_horizontal(float angle) override;
    void rotate_vertical(float angle) override;
    glm::vec3 direction() override;
private:
    GameObject* m_camera;

};
}
