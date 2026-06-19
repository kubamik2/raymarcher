#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "../objects/objects.hpp"
#include "gui/gui.hpp"
#include "input.hpp"
#include "imgui.h"
#include "../world.hpp"
#include "../shader/raymarcher.hpp"
#include "window.hpp"

namespace application {
class Application {
public:
    application::Input m_input;

    Application();
    ~Application();
    Application(const Application& app) = delete;

    static Application* instance() {
        if (m_instance) {
            return *m_instance;
        }
        m_instance = new Application{};
        return *m_instance;
    }

    void run();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void scroll_callback(GLFWwindow* window, double x, double y);

    void process_input(objects::Player *player, ImGuiIO& io, float dt);
private:
    static std::optional<Application*> m_instance;
    Window m_window;
    World m_world;
    shader::Raymarcher m_raymarcher;
    gui::Gui m_gui;
};
}
