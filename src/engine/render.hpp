#pragma once

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <map>

#include "shader.hpp"
#include "text_rendering.hpp"

#define WHITE glm::vec3(1.f, 1.f, 1.f)

enum class TypeBuffers {
    EBO = 0,
    VBO = 1,
    VAO = 2
};

enum class MapKey {
    OBJECTS = 0,
    TEXT = 1,
};

enum class AxisRotate {
    NONE   = -1,
    AXIS_X =  0,
    AXIS_Y =  1,
    AXIS_Z =  2,
};

// Класс Render - занимается отрисовкой сцены, расчетом матриц трансформаций и векторов
class Render {  
// Инвариат == ?
public:
    Render() = delete;

    explicit Render (const char* nw) : name_window_(nw) {}

    Render(const char* nw, unsigned int scr_w, unsigned int scr_h) : name_window_(nw), scr_width_(scr_w), scr_height_(scr_h) {}
    
    Render(const Render&) = delete;

    ~Render() {
        delete shader_;
    }

    inline GLFWwindow* GetWindow() const { return window_; }

    inline void SetWidthWindow(unsigned int w) { scr_width_ = w; }
    inline void SetHeightWindow(unsigned int h) { scr_height_ = h; }
    inline void SetNameWindow(const char* nw) { name_window_ = nw; }

    inline GLuint GetVBO(MapKey key) const { return vbo_.at(key); }
    inline GLuint GetVAO(MapKey key) const { return vao_.at(key); }
    inline GLuint GetEBO(MapKey key) const { return ebo_.at(key); }

    inline Shader* GetShaderPointer() const { return shader_; }

    inline unsigned int GetWindowWidth() const { return scr_width_; }
    inline unsigned int GetWindowHeight() const { return scr_height_; }

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    void InitWindow();
    void InitRender();
    void InitRenderText();
    void Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate);
    void DrawText(std::string text, float x, float y, float scale, glm::vec3 color);
    void SetOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);

private:
    GLFWwindow* CreateWindow(const char* nw, unsigned int scr_w, unsigned int scr_h);

    void GenerateBuffer(const GLsizei n, TypeBuffers type, MapKey key);
    void BufferData(GLenum target, GLsizeiptr sizeptr, const void* data, GLenum usage);

    void BindVertexArray(GLuint array);
    void BindBuffer(GLenum target, GLuint buffer);

    void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, void* offset);
    void EnableVertexAttribArray(GLuint index);

    glm::mat4 RotateMatrix(glm::mat4& model, AxisRotate axis, GLfloat rotate);
    glm::mat4 TranslateMatrix(glm::mat4& model, const glm::vec2& position);
    glm::mat4 ScaleMatrix(glm::mat4& model, const glm::vec2& size);

private:
    Shader* shader_;
    Shader* shader_text_;

    TypeBuffers type_buffers_;

    GLFWwindow* window_;
    const char* name_window_;
    unsigned int scr_width_ = 640;
    unsigned int scr_height_ = 480;

    std::map<MapKey, GLuint> vao_;
    std::map<MapKey, GLuint> vbo_;
    std::map<MapKey, GLuint> ebo_;

    TextRender* text_;

    // TODO: Хранить путь к шейдерам в классе рендера неправильно!
    const char* PATH_TO_FILE_VERTEX_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.vs";
    const char* PATH_TO_FILE_FRAGMENT_SHADER = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader.fs";
    // TODO: Хранить путь к шейдерам в классе рендера неправильно!
    const char* PATH_TO_FILE_VERTEX_SHADER_BALL = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader_ball.vs";
    const char* PATH_TO_FILE_FRAGMENT_SHADER_BALL = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/shader_ball.fs";

    // TODO: Хранить путь к шейдерам в классе рендера неправильно!
    const char* PATH_TO_VERTEX_SHADER_TEXT = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/text.vs";
    const char* PATH_TO_FRAGMENT_SHADER_TEXT = "/home/lpdgrl/Project/code/pingpong2D/src/shaders/text.fs";

    // TODO: Хранение пути в внутри классе рендера неправильно!
    const char* PATH_TO_FONT = "/home/lpdgrl/Project/code/pingpong2D/data/fonts/dejavusans.ttf";
};