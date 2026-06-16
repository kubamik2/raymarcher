#include "player.hpp"
#include "camera/camera.hpp"
#include "camera/center_camera.hpp"

objects::Player::Player() {
    objects::camera::Camera* camera = new objects::camera::CenterCamera{};
    add_child(camera);
    m_camera = camera;
}
