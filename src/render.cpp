#include "render.hpp"
#include <iostream>

void Render::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = CreateWindow(name_window_, scr_width_, scr_height_);

    glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
}

GLFWwindow* Render::CreateWindow(const char* nw, unsigned int scr_w, unsigned int scr_h) {
        GLFWwindow* window = glfwCreateWindow(scr_w, scr_h, nw, NULL, NULL);
        if (window == NULL) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            // return -1;
        }

        glfwMakeContextCurrent(window);
        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
        }  

        return window;
    }

// callback function for resizing window size
void Render::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}