#pragma once
#include <GLFW/glfw3.h>
namespace application {
class Window {
public: 
    Window();

    int width() const noexcept {
        return m_width;
    }

    int height() const noexcept {
        return m_height;
    }

    GLFWwindow* glfw_window() const noexcept {
        return m_window;
    }
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    int m_width, m_height;
private:
    GLFWwindow* m_window;
};
}
