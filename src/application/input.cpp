#include "input.hpp"

void application::Input::update_mouse_pos(double x, double y) {
    m_mouse_dx += m_mouse_pos_x - x;
    m_mouse_dy += m_mouse_pos_y - y;
    m_mouse_pos_x = x;
    m_mouse_pos_y = y;
}

void application::Input::update_mouse_scroll(double x, double y) {
    m_mouse_scroll_x += x;
    m_mouse_scroll_y += y;
}

void application::Input::reset_mouse_delta() {
    m_mouse_dx = 0.0;
    m_mouse_dy = 0.0;
}

void application::Input::reset_mouse_scroll() {
    m_mouse_scroll_x = 0.0;
    m_mouse_scroll_y = 0.0;
}
