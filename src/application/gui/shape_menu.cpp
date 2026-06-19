#include "shape_menu.hpp"
#include "imgui.h"
#include "../../objects/shapes/shape.hpp"
#include "../../objects/shapes/sphere.hpp"
#include "../../objects/shapes/box.hpp"
using namespace objects::shapes;

void gui::ShapeMenu::render(objects::shapes::Shape *shape) {
    ImGui::Begin("Shape Menu" );

    // translation
    glm::vec3 translation = shape->m_transform.translation();
    float pos[3]{translation.x, translation.y, translation.z};
    if (ImGui::DragFloat3("position", pos, 0.005f)) {
        translation.x = pos[0];
        translation.y = pos[1];
        translation.z = pos[2];
        shape->m_transform.translation(translation);
    }
    // rotation
    glm::vec3 angles = glm::eulerAngles(shape->m_transform.rotation());
    float r[3] = {glm::degrees(angles.x), glm::degrees(angles.y), glm::degrees(angles.z)};
    if (ImGui::DragFloat3("rotation", r, 0.5f)) {
        shape->m_transform.rotate(angles.x - glm::radians(r[0]), {1.0f, 0.0f, 0.0f});
        shape->m_transform.rotate(angles.y - glm::radians(r[1]), {0.0f, 1.0f, 0.0f});
        shape->m_transform.rotate(angles.z - glm::radians(r[2]), {0.0f, 0.0f, 1.0f});
    }

    // scale
    glm::vec3 scale = shape->m_transform.scaling();
    float s[3] = {scale.x, scale.y, scale.z};
    if (ImGui::DragFloat3("scale", s, 0.05f)) {
        scale.x = s[0];
        scale.y = s[1];
        scale.z = s[2];
        shape->m_transform.scaling(scale);
    }

    if (shape->type() == ShapeType::BOX) {
        Box* box = dynamic_cast<Box*>(shape);
        float size[3] = {box->m_size.x, box->m_size.y, box->m_size.z};
        if (ImGui::DragFloat3("size", size, 0.05f)) {
            box->m_size.x = size[0];
            box->m_size.y = size[1];
            box->m_size.z = size[2];
        }
    } else if (shape->type() == ShapeType::SPHERE) {
        Sphere* sphere = dynamic_cast<Sphere*>(shape);
        ImGui::DragFloat("radius", &sphere->m_radius, 0.05f);
    }

    // switch (shape->type()) {
    // case ShapeType::SPHERE:
    //     {
    //         Sphere* sphere = dynamic_cast<Sphere*>(shape);
    //         ImGui::DragFloat("radius", &sphere->m_radius, 0.05f);
    //     }
    //     break;
    // case ShapeType::BOX:
    //     Box* box = dynamic_cast<Box*>(shape);
    //     float size[3] = {box->m_size.x, box->m_size.y, box->m_size.z};
    //     if (ImGui::DragFloat("radius", size, 0.05f)) {
    //         box->m_size.x = size[0];
    //         box->m_size.y = size[1];
    //         box->m_size.z = size[2];
    //     }
    //     break;
    // }

    ImGui::End();
}
