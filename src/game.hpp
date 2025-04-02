#pragma once

#include "render.hpp"
#include "game_object.hpp"

static const SizeObject PLAYER_SIZE{5.f, 20.f};
static const PositionObject PLAYER_POSITION{400.f, 300.f};

// Класс отвечает за игровой цикл и обновление игровых объектов на сцене
// Инициализирует OpenGL
class Game {
public:
    Game() = default;

    // Game(const char* name_window) : name_window_(name_window) {}

    // Переделать инвариант - если разрешении меньше, бросаем исключение и завершаем работу.
    Game(const char* name_window, unsigned int scr_width, unsigned int scr_height) {
        // Инвариат == ?
        if (scr_width >= 640 && scr_height >= 480) {
            render_ = new Render(name_window, scr_width, scr_height);
            player_ = new GameObject(PLAYER_SIZE, {0.f, scr_height / 2 - PLAYER_SIZE.y / 2});
            return;
        }
         render_ = new Render(name_window);
         player_ = new GameObject(PLAYER_SIZE, {0.f, scr_height / 2 - PLAYER_SIZE.y / 2});
    }

    Game(const Game&) = delete;

    ~Game() {
        delete render_;
        delete player_;
        // delete ball_;
    };

    inline GLFWwindow* GetPointerWindow() const { return render_->GetWindow(); }

    // TODO: удалить в будущем из-за ненадобности
    inline Render* GetRenderPointer() const { return render_; }

public:
    void Update(const glm::vec2& position);
    void InitGame();
    void StartGame();
    void Render_(const glm::vec2& position, const glm::vec2& size, float rotate);

private:
    Render* render_;
    GameObject* player_;
    GameObject* ball_;
};