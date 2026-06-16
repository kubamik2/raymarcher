#pragma once
#include "camera/camera.hpp"
#include "game_object.hpp"

namespace objects {
class Player : public GameObject {
public:
    Player();
    objects::camera::Camera *m_camera;
private:

};
}
