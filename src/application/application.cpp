#include "application.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdio>
#include <format>
#include <string>
#include "../shader/shader.hpp"
#include "../shader/shader_builder.hpp"
#include "../screen.hpp"

#include "glm/ext/vector_bool3_precision.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "../objects/shapes/sphere.hpp"
#include "../objects/shapes/box.hpp"

std::optional<application::Application*> application::Application::m_instance{};
application::Application::Application() {
    // callbacks
    glfwSetFramebufferSizeCallback(m_window.glfw_window(), this->framebuffer_size_callback);
    glfwSetScrollCallback(m_window.glfw_window(), Application::scroll_callback);
    glfwSetCursorPosCallback(m_window.glfw_window(), Application::cursor_position_callback);
}

application::Application::~Application() {
    glfwTerminate();
}

objects::GameObject* selected = nullptr;
void list(objects::GameObject* object) {
    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (selected == object) {
        base_flags |= ImGuiTreeNodeFlags_Selected; 
    }

    std::string name = std::string{object->name()} + std::format("#{}", object->id());
    bool is_open = ImGui::TreeNodeEx(name.c_str(), base_flags);
    if ((ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right)) && !ImGui::IsItemToggledOpen()) {
        selected = object;
    }

    if (ImGui::BeginPopupContextItem(0)) {
        if (ImGui::MenuItem("Add sphere")) {
            auto sphere = new objects::shapes::Sphere{{}, 1.0f};
            selected->add_child(sphere);
        }
        if (ImGui::MenuItem("Add box")) {
            auto box = new objects::shapes::Box{{}, {1.0, 1.0, 1.0}};
            selected->add_child(box);
        }
        if (ImGui::MenuItem("Delete")) {
            selected->parent()->remove_child(selected);
            selected = nullptr;
        }
        ImGui::EndPopup();
    }

    if (is_open) {
        for (auto child : object->children()) {
            list(child);
        }
        ImGui::TreePop();
    }
}

void menu() {
    if (selected != nullptr) {
        glm::vec3 translation = selected->m_transform.translation();
        float pos[3]{translation.x, translation.y, translation.z};
        if (ImGui::DragFloat3("position", pos, 0.005f)) {
            translation.x = pos[0];
            translation.y = pos[1];
            translation.z = pos[2];
            selected->m_transform.translation(translation);
        }
        glm::vec3 angles = glm::eulerAngles(selected->m_transform.rotation());
        float r[3] = {glm::degrees(angles.x), glm::degrees(angles.y), glm::degrees(angles.z)};
        if (ImGui::DragFloat3("rotation", r, 0.5f)) {
            selected->m_transform.rotate(angles.x - glm::radians(r[0]), {1.0f, 0.0f, 0.0f});
            selected->m_transform.rotate(angles.y - glm::radians(r[1]), {0.0f, 1.0f, 0.0f});
            selected->m_transform.rotate(angles.z - glm::radians(r[2]), {0.0f, 0.0f, 1.0f});
        }
    }
}

void application::Application::run() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window.glfw_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    float dt;
    float last_frame = (float)glfwGetTime();
    // render loop
    while (!glfwWindowShouldClose(m_window.glfw_window())) {
        // printf("%.1f\n", 1.0f / dt);
        process_input(m_world.player(), io, dt);

        m_raymarcher.render(m_world, m_window.m_width, m_window.m_height);

        m_gui.render(m_world);

        m_input.reset_mouse_delta();
        m_input.reset_mouse_scroll();
        // swap and poll
        glfwSwapBuffers(m_window.glfw_window());
        glfwPollEvents();
        dt = glfwGetTime() - last_frame;
        last_frame = glfwGetTime();
    }
}

void application::Application::process_input(objects::Player *player, ImGuiIO& io, float dt) {
    if (m_input.get_key(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window.glfw_window(), true);
    }
    if (m_input.get_key(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        player->m_transform.translate(player->m_camera->direction() * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        player->m_transform.translate(-player->m_camera->direction() * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 side = glm::cross(player->m_camera->direction(), glm::vec3(0.0, 1.0, 0.0));
        player->m_transform.translate(-side * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 side = glm::cross(player->m_camera->direction(), glm::vec3(0.0, 1.0, 0.0));
        player->m_transform.translate(side * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player->m_transform.translate(glm::vec3(0.0, 1.0, 0.0) * dt);
    }
    if (m_input.get_key(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        player->m_transform.translate(-glm::vec3(0.0, 1.0, 0.0) * dt);
    }

    if (!m_input.gui_wants_mouse()) {
        if (m_input.get_mouse_button(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            player->m_camera->rotate_horizontal(glm::radians(m_input.mouse_dx()));
            player->m_camera->rotate_vertical(glm::radians(m_input.mouse_dy()));
        }

        player->m_camera->m_transform.scale(glm::vec3(1.0f + (3.0 * -m_input.mouse_scroll_y() * dt)));
    }

}

void application::Application::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    Application* app = Application::instance();
    app->m_window.m_width = width;
    app->m_window.m_height = height;
}

void application::Application::cursor_position_callback(GLFWwindow *window, double x, double y) {
    Application* app = Application::instance();
    app->m_input.update_mouse_pos(x, y);
}

void application::Application::scroll_callback(GLFWwindow *window, double x, double y) {
    Application* app = Application::instance();
    app->m_input.update_mouse_scroll(x, y);
}
