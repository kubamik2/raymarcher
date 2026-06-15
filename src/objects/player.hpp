#pragma once
#include "camera.hpp"
#include "game_object.hpp"

namespace objects {
class Player : public GameObject {
public:
    Player();
    objects::Camera *m_camera;
private:

};
}
