#include "gui.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <iostream>

gui::Gui::Gui() {
    ImGui::CreateContext();
    // ImGui::GetStyle().ScaleAllSizes(4.0);
}

void gui::Gui::render(World &world) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    m_shape_tree.render(world);
    if (m_shape_tree.selected().has_value()) {
        m_shape_menu.render(m_shape_tree.selected().value());
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
