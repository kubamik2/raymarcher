#include "player.hpp"
#include "camera/camera.hpp"
#include "camera/center_camera.hpp"
#include "game_object.hpp"

objects::Player::Player(glm::vec3 position) : objects::GameObject(position) {
    objects::camera::Camera* camera = new objects::camera::CenterCamera{{}};
    add_child(camera);
    m_camera = camera;
}
