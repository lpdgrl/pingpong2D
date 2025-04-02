#include "render.hpp"
#include <iostream>

void Render::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = CreateWindow(name_window_, scr_width_, scr_height_);

    glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);

    glEnable(GL_DEPTH_TEST);
    shader_ = new Shader(PATH_TO_FILE_VERTEX_SHADER, PATH_TO_FILE_FRAGMENT_SHADER);
}

void Render::InitRender() {
    float vertices_player[] = {
        // positions         // colors
        50.f,  50.f, 0.0f,  1.0f, 1.0f, 1.0f,  // top right
        50.f,  -50.f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom right
        -50.f,  50.f, 0.0f,  .0f, 1.0f, .0f,  // bottom left
        -50.f, -50.f, 0.0f,  .0f, 1.0f, .0f   // top left 
    };

    unsigned int indices_player[] = {
        0, 1, 3,
        0, 3, 2
    };

    GenerateBuffers(1, TypeBuffers::VAO);
    GenerateBuffers(1, TypeBuffers::VBO);
    GenerateBuffers(1, TypeBuffers::EBO);

    BindVertexArray(GetVAO(0));
    BindBuffer(GL_ARRAY_BUFFER, GetVBO(0));
    BufferData(GL_ARRAY_BUFFER, sizeof(vertices_player), vertices_player, GL_DYNAMIC_DRAW);

    BindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetEBO(0));
    BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_player), indices_player, GL_DYNAMIC_DRAW);
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

void Render::GenerateBuffers(const GLsizei n, TypeBuffers type) {
    GLuint b;
    
    switch(type)
    {
        case TypeBuffers::VAO: 
            glGenVertexArrays(n, &b);
            vao_.push_back(b);
        break;

        case TypeBuffers::VBO:
            glGenBuffers(n, &b);
            vbo_.push_back(b);
        break;

        case TypeBuffers::EBO:
            glGenBuffers(n, &b);
            ebo_.push_back(b);
        break;
    }
}

void Render::BindVertexArray(GLuint array) {
    glBindVertexArray(array);
}

void Render::BindBuffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void Render::BufferData(GLenum target, GLsizeiptr sizeptr, const void* data, GLenum usage) {
    glBufferData(target, sizeptr, data, usage);
}

void Render::Draw(const glm::vec2& position) {
    shader_->use();
    BindVertexArray(GetVAO(0));
    glm::mat4 model_matrix = glm::mat4(1.f);
    model_matrix = glm::translate(model_matrix, glm::vec3(position, 0.f));

    shader_->setMat4("modelMat", model_matrix);

    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
}