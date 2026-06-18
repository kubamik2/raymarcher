#include "application.hpp"
#include "window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

application::Window::Window() {
    // initialize glfw
    if (!glfwInit()) {
        throw std::runtime_error("GLFW couldn't init");
    }

    // hint OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // get monitor details
    auto monitor = glfwGetPrimaryMonitor();
    const auto mode = glfwGetVideoMode(monitor);
    m_width = mode->width;
    m_height = mode->height;
    
    // create a window
    m_window = glfwCreateWindow(m_width, m_height, "Window", monitor, NULL);
    if (m_window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(true);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, m_width, m_height);
}
