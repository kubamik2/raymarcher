#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "../objects/objects.hpp"

namespace application {
class Application {
public:
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

    static void process_input(GLFWwindow* window, objects::Player &player, float dt);
private:
    static std::optional<Application*> m_instance;
    GLFWwindow *m_window;
    int m_width, m_height;
};
}
