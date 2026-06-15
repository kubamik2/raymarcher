#include "player.hpp"
#include "camera.hpp"

objects::Player::Player() {
    objects::Camera* camera = new objects::Camera {};
    add_child(camera);
    m_camera = camera;
}
