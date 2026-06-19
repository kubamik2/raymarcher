#include "world.hpp"
#include "objects/shapes/sphere.hpp"
#include "objects/shapes/box.hpp"

World::World() {
    m_shapes = new objects::shapes::ShapeHolder{};
    m_root.add_child(m_shapes);

    m_player = new objects::Player{{}};
    m_root.add_child(m_player);

    auto sphere1 = new objects::shapes::Sphere{
    {{0.2, 0.5, 0.2}},
        1.0
    };
    m_shapes->add_child(sphere1);

    auto sphere2 = new objects::shapes::Sphere{
        {{0.2, 0.0, 1.0}},
        0.5
    };
    m_shapes->add_child(sphere2);

    auto box = new objects::shapes::Box{
        {{0.0, 2.0, 0.0}},
        {0.5, 0.5, 0.5}
    };
    m_shapes->add_child(box);
}
