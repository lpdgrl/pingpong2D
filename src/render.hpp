#pragma once

#include <glad.h>
#include <glfw3.h>

// Класс Render - занимается отрисовкой сцены, расчетом матриц трансформаций и векторов
// 

class Render {  
// Инвариат == ?
public:
    Render() = default;

    Render (const char* nw) : name_window_(nw) {}

    Render(const char* nw, unsigned int scr_w, unsigned int scr_h) : name_window_(nw), scr_width_(scr_w), scr_height_(scr_h) {}

    ~Render() = default;

    inline GLFWwindow* GetWindow() const { return window_; }

    inline void SetWidthWindow(unsigned int w) { scr_width_ = w; }
    inline void SetHeightWindow(unsigned int h) { scr_height_ = h; }
    inline void SetNameWindow(const char* nw) { name_window_ = nw; }

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    void InitWindow();

private:
    GLFWwindow* CreateWindow(const char* nw, unsigned int scr_w, unsigned int scr_h);


private:
    GLFWwindow* window_;
    const char* name_window_;
    unsigned int scr_width_ = 640;
    unsigned int scr_height_ = 480;
};