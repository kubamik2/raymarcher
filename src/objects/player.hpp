#pragma once
#include "camera/camera.hpp"
#include "game_object.hpp"

namespace objects {
class Player : public GameObject {
public:
    Player(glm::vec3 position);
    objects::camera::Camera *m_camera;
    const char* name() override {
        return "Player";
    };
private:

};
}
