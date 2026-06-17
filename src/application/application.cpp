#include "application.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <stdexcept>
#include "../shader/shader.hpp"
#include "../shader/shader_builder.hpp"
#include "../screen.hpp"

std::optional<application::Application*> application::Application::m_instance{};

application::Application::Application() {
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

    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // if (glfwRawMouseMotionSupported()) {
    //     glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    // }

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, m_width, m_height);

    // callbacks
    glfwSetFramebufferSizeCallback(m_window, this->framebuffer_size_callback);
    glfwSetScrollCallback(m_window, Application::scroll_callback);
    glfwSetCursorPosCallback(m_window, Application::cursor_position_callback);

}

application::Application::~Application() {
    glfwTerminate();
}

void application::Application::run() {
    Screen screen{ "src/shaders/vertex.glsl", "src/shaders/fragment.glsl" };
    Shader compute_shader = ShaderBuilder{}
        .add_module("src/shaders/compute.glsl", GL_COMPUTE_SHADER)
        .build();

    objects::Player player;

    float dt;
    float last_frame = (float)glfwGetTime();
    // render loop
    while (!glfwWindowShouldClose(m_window)) {
        // input
        process_input(player, dt);

        // execute raymarching compute shader
        {
            compute_shader.use();
            player.m_camera->bind(compute_shader.get_id());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screen.get_screen_texture());
            glUniform1i(glGetUniformLocation(compute_shader.get_id(), "screen"), 0);
            glDispatchCompute(m_width, m_height, 1);
        }

        // wait for compute shader
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        // draw results
        {
            glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            screen.render();
        }

        m_input.reset_mouse_delta();
        m_input.reset_mouse_scroll();
        // swap and poll
        glfwSwapBuffers(m_window);
        glfwPollEvents();
        dt = glfwGetTime() - last_frame;
        last_frame = glfwGetTime();
    }
}

void application::Application::process_input(objects::Player &player, float dt) {
    if (m_input.get_key(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
    if (m_input.get_key(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        player.m_transform.translate(player.m_camera->direction() * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        player.m_transform.translate(-player.m_camera->direction() * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 side = glm::cross(player.m_camera->direction(), glm::vec3(0.0, 1.0, 0.0));
        player.m_transform.translate(-side * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 side = glm::cross(player.m_camera->direction(), glm::vec3(0.0, 1.0, 0.0));
        player.m_transform.translate(side * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player.m_transform.translate(glm::vec3(0.0, 1.0, 0.0) * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        player.m_transform.translate(-glm::vec3(0.0, 1.0, 0.0) * dt);
    }

    if (m_input.get_mouse_button(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        player.m_camera->rotate_horizontal(glm::radians(m_input.mouse_dx()));
        player.m_camera->rotate_vertical(glm::radians(m_input.mouse_dy()));
    }


    player.m_camera->m_transform.scale(glm::vec3(1.0f + (3.0 * m_input.mouse_scroll_y() * dt)));
}

void application::Application::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    Application* app = Application::instance();
    app->m_width = width;
    app->m_height = height;
}

void application::Application::cursor_position_callback(GLFWwindow *window, double x, double y) {
    Application* app = Application::instance();
    app->m_input.update_mouse_pos(x, y);
}

void application::Application::scroll_callback(GLFWwindow *window, double x, double y) {
    Application* app = Application::instance();
    app->m_input.update_mouse_scroll(x, y);
}
