#include "application.hpp"
#include <GLFW/glfw3.h>
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

#define WORK_GROUP_SIZE 16

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
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        selected = object;
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
    }
}


// struct shape {
//     int type;
//     float _[3];
//     float x[4];
//     float y[4];
// };

void application::Application::run() {
    // Screen screen{ "src/shaders/vertex.glsl", "src/shaders/fragment.glsl" };
    // Shader compute_shader = ShaderBuilder{}
    //     .add_module("src/shaders/compute.glsl", GL_COMPUTE_SHADER)
    //     .build();
    //
    // objects::Player player;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window.glfw_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    // struct shape s;
    // s.type = 0;
    // s.x[0] = 0.0f;
    // s.x[1] = 0.0f;
    // s.x[2] = 0.0f;
    // s.x[3] = 1.0f;
    //
    // struct data_t {
    //     shape shapes[3];
    // } data;
    // data.shapes[0] = s;
    // s.x[0] = 1.9;
    // data.shapes[1] = s;
    //
    // s.type = 1;
    // s.x[0] = 2.0f;
    // s.x[1] = 2.0f;
    // s.x[2] = 0.0f;
    // s.x[3] = 0.5f;
    // s.y[0] = 0.5f;
    // s.y[1] = 0.5f;
    // data.shapes[2] = s;
    // GLuint ssbo;
    // glGenBuffers(1, &ssbo);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), &data, GL_DYNAMIC_COPY);

    float dt;
    float last_frame = (float)glfwGetTime();
    // render loop
    while (!glfwWindowShouldClose(m_window.glfw_window())) {
        // input
        process_input(m_world.player(), io, dt);

        m_raymarcher.render(m_world, m_window.m_width, m_window.m_height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::BeginViewportSideBar("My name is window, ImGUI window", viewport, ImGuiDir_Right, 200.0f, flags);
        ImGui::Text("%.1f", 1.0f / dt);
        list(m_world.shapes());
        menu();


        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



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


        player->m_camera->m_transform.scale(glm::vec3(1.0f + (3.0 * m_input.mouse_scroll_y() * dt)));
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
