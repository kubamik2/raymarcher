#include "camera/camera.hpp"
#include "glm/trigonometric.hpp"
#include "screen.hpp"
#include "shader/shader_builder.hpp"
#include "shader/shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1200

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera &camera, float dt);
void run(GLFWwindow* window);

GLfloat VERTICES[] = {
    -1.0, -1.0, 0.0, 0.0, 0.0,
    -1.0, 1.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 1.0, 1.0,
    1.0, -1.0, 0.0, 1.0, 0.0
};

GLuint INDICES[] = {
    0, 2, 1,
    0, 3, 2
};

int main() {
    // initialize glfw
    if (!glfwInit()) {
        std::printf("GLFW couldn't init\n");
        return -1;
    }

    // hint OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // create a window
    GLFWwindow* window;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Window", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(true);

    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // window resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    run(window);
    
    // cleanup
    glfwTerminate();
    return 0;
}

void run(GLFWwindow* window) {
    Screen screen { "src/shaders/vertex.glsl", "src/shaders/fragment.glsl" };
    Shader compute_shader = ShaderBuilder {}
        .add_module("src/shaders/compute.glsl", GL_COMPUTE_SHADER)
        .build();

    Camera camera;

    float dt;
    float last_frame = (float)glfwGetTime();
    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window, camera, dt);

        // rendering
        {
            compute_shader.use();
            camera.bind(compute_shader.get_id());
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screen.get_screen_texture());
            glUniform1i(glGetUniformLocation(compute_shader.get_id(), "screen"), 0);
            glDispatchCompute(WINDOW_WIDTH, WINDOW_HEIGHT, 1);
        }

        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        {
            glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            screen.render();
        }

        // swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
        dt = glfwGetTime() - last_frame;
        last_frame = glfwGetTime();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, Camera &camera, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.position += camera.getDirection() * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position -= camera.getDirection() * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 side = glm::cross(camera.getDirection(), glm::vec3(0.0, 1.0, 0.0));
        camera.position -= side * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 side = glm::cross(camera.getDirection(), glm::vec3(0.0, 1.0, 0.0));
        camera.position += side * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.position += glm::vec3(0.0, 1.0, 0.0) * dt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.position -= glm::vec3(0.0, 1.0, 0.0) * dt;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.rotate(glm::radians(-1.0), glm::vec3(0.0, 1.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.rotate(glm::radians(1.0), glm::vec3(0.0, 1.0, 0.0));
    }
}
