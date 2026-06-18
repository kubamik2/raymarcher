#pragma once
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "window.hpp"

namespace application {
class Input {
public:
    int get_key(Window &window, int key) const {
        return glfwGetKey(window.glfw_window(), key);
    }

    int get_mouse_button(Window &window, int button) const {
        return glfwGetMouseButton(window.glfw_window(), button);
    }

    double mouse_dx() const noexcept {
        return m_mouse_dx;
    }

    double mouse_dy() const noexcept {
        return m_mouse_dy;
    }

    double mouse_x() const noexcept {
        return m_mouse_pos_x;
    }

    double mouse_y() const noexcept {
        return m_mouse_pos_y;
    }

    double mouse_scroll_x() const noexcept {
        return m_mouse_scroll_x;
    }

    double mouse_scroll_y() const noexcept {
        return m_mouse_scroll_y;
    }

    bool gui_wants_mouse() const {
        return ImGui::GetIO().WantCaptureMouse;
    }

    void update_mouse_pos(double x, double y);
    void update_mouse_scroll(double x, double y);

    void reset_mouse_delta();
    void reset_mouse_scroll();
private:
    // mouse
    double m_mouse_pos_x, m_mouse_pos_y;
    double m_mouse_dx, m_mouse_dy;
    double m_mouse_scroll_x, m_mouse_scroll_y;
};
}
