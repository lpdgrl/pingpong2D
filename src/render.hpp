#pragma once

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.hpp"


enum class TypeBuffers {
    EBO = 0,
    VBO = 1,
    VAO = 2
};


// Класс Render - занимается отрисовкой сцены, расчетом матриц трансформаций и векторов
class Render {  
// Инвариат == ?
public:
    Render() = default;

    explicit Render (const char* nw) : name_window_(nw) {}

    Render(const char* nw, unsigned int scr_w, unsigned int scr_h) : name_window_(nw), scr_width_(scr_w), scr_height_(scr_h) {
        
        
    }

    ~Render() {
        //delete shader_;
    }

    inline GLFWwindow* GetWindow() const { return window_; }

    inline void SetWidthWindow(unsigned int w) { scr_width_ = w; }
    inline void SetHeightWindow(unsigned int h) { scr_height_ = h; }
    inline void SetNameWindow(const char* nw) { name_window_ = nw; }

    inline GLuint GetVBO(size_t i) const { return vbo_[i]; }
    inline GLuint GetVAO(size_t i) const { return vao_[i]; }
    inline GLuint GetEBO(size_t i) const { return ebo_[i]; }

    inline Shader* GetShaderPointer() const { return shader_; }

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    void InitWindow();
    void InitRender();
    void Draw(const glm::vec2& position);

private:
    GLFWwindow* CreateWindow(const char* nw, unsigned int scr_w, unsigned int scr_h);

    void GenerateBuffers(const GLsizei n, TypeBuffers type);

    void BindVertexArray(GLuint array);
    void BindBuffer(GLenum target, GLuint buffer);

    void BufferData(GLenum target, GLsizeiptr sizeptr, const void* data, GLenum usage);

private:
    Shader* shader_;

    TypeBuffers type_buffers_;

    GLFWwindow* window_;
    const char* name_window_;
    unsigned int scr_width_ = 640;
    unsigned int scr_height_ = 480;

    std::vector<GLuint> vao_;
    std::vector<GLuint> vbo_;
    std::vector<GLuint> ebo_;

    // TODO: Хранить путь к шейдерам в классе рендера неправильно!
    const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.vs";
    const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.fs";
    // TODO: Хранить путь к шейдерам в классе рендера неправильно!
    const char* PATH_TO_FILE_VERTEX_SHADER_BALL = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader_ball.vs";
    const char* PATH_TO_FILE_FRAGMENT_SHADER_BALL = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader_ball.fs";
};