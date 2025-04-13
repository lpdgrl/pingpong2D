#pragma once

#include "../engine/render.hpp"
#include "game_object.hpp"
#include "ball.hpp"
#include "player.hpp"

enum class KeyPress {
    W = 0,
    S = 1,
    R = 2,
    ENTER = 3,
    SPACE = 4,
    None = 5
};

enum class DirectionBall {
    LEFT = -1,
    NOWHERE = 0,
    RIGHT = 1,
};

enum class DirectionPlayer {
    DOWN = -1,
    NOWHERE = 0,
    UP = 1,
};

static const SizeObject PLAYER_SIZE{20.f, 100.f};
static const PositionObject PLAYER_POSITION{400.f, 300.f};

static const SizeObject BALL_SIZE{10.f, 10.f};

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
            player_ = new Player(PLAYER_SIZE, {0.f, scr_height / 2 - PLAYER_SIZE.y / 2}, {0.f, 500.f}, static_cast<int>(DirectionPlayer::NOWHERE),
                                static_cast<int>(DirectionPlayer::NOWHERE));
            ball_ = new Ball(BALL_SIZE, {scr_width / 2 - BALL_SIZE.x, scr_height / 2 - BALL_SIZE.y}, {100.f, 80.f}, static_cast<int>(DirectionBall::LEFT),
                            static_cast<int>(DirectionBall::NOWHERE), false);
        }
    }

    Game(const Game&) = delete;
    Game& operator=(const Game& game) = delete;

    Game(Game&& game) = delete;
    Game& operator=(Game&& game) = delete;

    ~Game() {
        delete render_;
        delete player_;
        delete ball_;
    };

    inline GLFWwindow* GetPointerWindow() const { return render_->GetWindow(); }

    // TODO: удалить в будущем из-за ненадобности
    inline Render* GetRenderPointer() const { return render_; }

public:
    void Update(GLfloat dt);
    void InitGame();
    void StartGame();
    void ResetGame();
    void Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate);
    bool CheckColision(GameObject* one, GameObject* two);
    void GameLog(GLfloat parametr) const;

private:
    KeyPress ProcessInput(GLFWwindow* window);
    glm::vec2 GetPositionObj(GameObject* obj);
    glm::vec2 GetSizeObj(GameObject* obj);

private:
    Render* render_ = nullptr;
    Player* player_ = nullptr;
    Ball* ball_ = nullptr;
    bool start_game_ = false;
};