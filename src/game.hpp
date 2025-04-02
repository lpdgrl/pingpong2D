#pragma once

#include "render.hpp"

// Класс отвечает за игровой цикл и обновление игровых объектов на сцене
// Инициализирует OpenGL
// Инвариат == ?
class Game {
public:
    Game() = default;

    // Game(const char* name_window) : name_window_(name_window) {}

    Game(const char* name_window, unsigned int scr_width, unsigned int scr_height) {
        if (scr_width >= 640 && scr_height >= 480) {
            render_ = new Render(name_window, scr_width, scr_height);
            return;
        }
         render_ = new Render(name_window);
    }

    ~Game() {
        delete render_;
    };

    inline GLFWwindow* GetPointerWindow() const { return render_->GetWindow(); }

    inline Render* GetRenderPointer() const { return render_; }

public:
    void Update(const glm::vec2& position);
    void InitGame();
    void StartGame();
    void Render_(const glm::vec2& position);

private:
    Render* render_;
};