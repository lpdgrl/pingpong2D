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

public:
    void Update();
    void InitGame();
    void StartGame();

private:
    Render* render_;
};