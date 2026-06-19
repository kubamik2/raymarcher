#include "shape_tree.hpp"
#include "imgui.h"
#include <cstdio>
#include <format>
#include <iostream>
#include <string>
#include "../../objects/shapes/box.hpp"
#include "../../objects/shapes/sphere.hpp"
#include "../../objects/shapes/fractal.hpp"

using gui::ShapeTree, std::optional;
using namespace objects::shapes;


void ShapeTree::draw(objects::GameObject* object) {
    auto shape = dynamic_cast<Shape*>(object);
    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (m_selected == shape) {
        base_flags |= ImGuiTreeNodeFlags_Selected; 
    }

    std::string name = std::format("{}#{}", shape->name(), shape->id());
    bool is_open = ImGui::TreeNodeEx(name.c_str(), base_flags);
    if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right)) && !ImGui::IsItemToggledOpen()) {
        m_selected = shape;
        m_popup_handled = true;
    }

    if (ImGui::BeginPopupContextItem(0) && m_selected.has_value()) {
        Shape *selected = m_selected.value();
        if (ImGui::MenuItem("Add sphere")) {
            auto sphere = new Sphere{{}, 1.0f};
            selected->add_child(sphere);
        }
        if (ImGui::MenuItem("Add box")) {
            auto box = new objects::shapes::Box{{}, {1.0, 1.0, 1.0}};
            selected->add_child(box);
        }
        if (ImGui::MenuItem("Add fractal")) {
            auto fractal = new objects::shapes::Fractal{{}};
            selected->add_child(fractal);
        }
        if (ImGui::MenuItem("Delete")) {
            m_delete_selected = true;

        }
        ImGui::EndPopup();
    }

    if (is_open) {
        for (auto child : shape->children()) {
            this->draw(dynamic_cast<Shape*>(child));
        }
        ImGui::TreePop();
    }
}

void ShapeTree::render(World &world) {
    ImGui::Begin("Shape Tree" );
    for (auto child : world.shapes()->children()) {
        this->draw(child);
    }
    if (m_delete_selected) {
        m_delete_selected = false;
        Shape *selected = m_selected.value();
        selected->parent()->remove_child(selected);
        m_selected = {};
    }

    // was background clicked
    if (
        ImGui::IsWindowHovered()
        && !m_popup_handled
    ) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            m_selected = {};
        } else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            // show right click menu
            ImGui::OpenPopup("background context");
        } 
    }
    if (ImGui::BeginPopup("background context") && !m_popup_handled) {
        if (ImGui::MenuItem("Add sphere")) {
            auto sphere = new Sphere{{}, 1.0f};
            world.shapes()->add_child(sphere);
        }
        if (ImGui::MenuItem("Add box")) {
            auto box = new objects::shapes::Box{{}, {1.0, 1.0, 1.0}};
            world.shapes()->add_child(box);
        }
        if (ImGui::MenuItem("Add fractal")) {
            auto fractal = new objects::shapes::Fractal{{}};
            world.shapes()->add_child(fractal);
        }
        m_popup_handled = true;
        ImGui::EndPopup();
    }
    ImGui::End();
    m_popup_handled = false;
}
