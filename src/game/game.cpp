#include "game.hpp"
#include <random>

void Game::InitGame() {
    render_->InitWindow();
    render_->InitRender();
    render_->InitRenderText();

    // Initialazation game log
    std::vector<std::string> log_param = {"PosY player1", "PosY player2", "PosX ball", 
                                            "PosY ball", "VelX ball", "VelY ball", "DirX ball", "DirY ball",
                                            "Delta time", "Rotate ball", "Collision player1", "Collision player2"};
    Logger::InitLogArray(log_param);
}

void Game::Update() {
    render_->DrawText("PING PONG 2D", 300.0f, 575.0f, 0.5f, WHITE);
    render_->DrawText(std::to_string(player_one_->GetScore()), 120.f, 550., 0.5f, WHITE);
    render_->DrawText(std::to_string(player_two_->GetScore()), 700.f, 550., 0.5f, WHITE);

    auto keys = controller_->GetPressKeys();

    if (keys[static_cast<int>(KeyPress::SPACE)]) {
        start_game_ = true;
    }
    if (keys[static_cast<int>(KeyPress::R)]) {
        ResetGame();
    }

    bool wasColliding = ball_->GetCollPlayerOne();
    bool wasCollPlayerTwo = ball_->GetCollPlayerTwo();

    int directionX_ball = ball_->GetDirectionX();
    int directionY_ball = ball_->GetDirectionY();

    GLfloat offset = ball_->GetOffset();

    glm::vec2 coord_ball = ball_->GetCoordVec();
    glm::vec2 size_ball = ball_->GetSizeVec();

    // Перезапуск игры если мяч вышел за пределы
    if (coord_ball.x + size_ball.x < 0.f ) {
        player_two_->SetWinner(true);
        ResetGame();
    }
    else if (coord_ball.x + size_ball.x > render_->GetWindowWidth()) {
        player_one_->SetWinner(true);
        ResetGame();
    }
    
    if (start_game_) {
        // чекаем коллизию
        bool collision_player_one = CheckCollision(player_one_, ball_, false);
        bool collision_player_two = CheckCollision(player_two_, ball_, true);

        // проверяем была ли коллизия (переписать в метод)
        if (collision_player_one && !wasColliding) {
            directionX_ball *= -1;
            ball_->SetDirectionX(directionX_ball);
            ball_->SetDirectionY(player_one_->GetDirectionY());
            GLfloat offset = 0.f;
            if (player_one_->GetDirectionY() > 0 || 0 < player_one_->GetDirectionY()) {
                offset = 100.f * dt_;    
            } else if (player_one_->GetDirectionY() == 0) {
                offset = 20.f * dt_;
            }
            ball_->SetOffset(offset);
        }
        
        if (collision_player_two && !wasCollPlayerTwo) {
            directionX_ball *= -1;
            ball_->SetDirectionX(directionX_ball);
            ball_->SetDirectionY(player_two_->GetDirectionY());
            GLfloat offset = 0.f;
            if (player_two_->GetDirectionY() > 0 || 0 < player_two_->GetDirectionY()) {
                offset = 100.f * dt_;
            } else if (player_two_->GetDirectionY() == 0) {
                offset = 20.f * dt_;
            }

            ball_->SetOffset(offset);
        }

        // устанавливаем состояние проверки коллизии
        ball_->SetCollPlayerOne(collision_player_one); 
        ball_->SetCollPlayerTwo(collision_player_two);

        // чекаем на совпадение границ экрана с координатами мяча, если совпадает то меняем направление
        if (coord_ball.y + size_ball.y >= render_->GetWindowHeight()) {
            ball_->SetDirectionY(static_cast<int>(DirectionBall::LEFT));
        } 
        else if (coord_ball.y + size_ball.y <= 0) {
            ball_->SetDirectionY(static_cast<int>(DirectionBall::RIGHT));
        }
        
        MoveBall();
        MovePlayer(keys);
    }

    // Отрисовываем игровые объекты
    player_one_->DrawObject();
    player_two_->DrawObject();
    
    ball_->SetRotate(std::sin(glfwGetTime()) * 200.f);
    ball_->DrawObject(AxisRotate::AXIS_Z, ball_->GetRotate());
    
    if (keys[static_cast<int>(KeyPress::Q)]) {
        debug_mode_ = !debug_mode_;
    }
    
    if (debug_mode_) {
        UpdateDataLog();
    }
}

void Game::MoveBall() {
    ball_->Move(GetDt());
}

void Game::MovePlayer(std::array<bool, SIZE_ARRAY_KEYS>& keys) {
      // обрабатываем кнопки движения игрока (возможно стоит переписывать в метод move для игрового объекта типа игрок)
        if (keys[static_cast<int>(KeyPress::W)]) {
            if ((player_one_->GetCoordVec().y + player_one_->GetSizeVec().y) <= render_->GetWindowHeight()) {
                player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::UP));
                player_one_->Move(GetDt());
            }
        }
            
        if (keys[static_cast<int>(KeyPress::S)]) {
            if ((player_one_->GetCoordVec().y - player_one_->GetSizeVec().y) >= 0) {
                player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::DOWN));
                player_one_->Move(GetDt());
            }
        }

        if (keys[static_cast<int>(KeyPress::I)]) {
            if ((player_two_->GetCoordVec().y + player_two_->GetSizeVec().y) <= render_->GetWindowHeight()) {
                player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::UP));
                player_two_->Move(GetDt());
            }
        }

        if (keys[static_cast<int>(KeyPress::K)]) {
            if ((player_two_->GetCoordVec().y - player_two_->GetSizeVec().y) >= 0) {
                player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::DOWN));
                player_two_->Move(GetDt());
            }
        }
            //player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));
            //player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));
}

bool Game::CheckCollision(GameObject* one, GameObject* two, bool second_player) {
    glm::vec2 coord_one = one->GetCoordVec();
    glm::vec2 size_one = one->GetSizeVec();

    glm::vec2 coord_two = two->GetCoordVec();
    glm::vec2 size_two = two->GetSizeVec();
    
    bool collisionX = false;
    if (!second_player) {
        collisionX = coord_two.x - size_two.x <= coord_one.x + size_one.x;
    }
    else {
        collisionX = coord_two.x + size_two.x >= coord_one.x - size_one.x;
    }

    // разобраться как убрать голые значения - используются для того, чтобы учитывать вверхний и нижний игрока
    bool collisionY = coord_two.y + size_two.y <= coord_one.y + size_one.y + 15.f && coord_two.y - size_two.y >= coord_one.y - size_one.y - 15.f;

    return collisionX && collisionY;
}

void Game::StartGame() {
    render_->SetOrthoProjection(0.f, 0.f, 0.f, 0.f, -50.f, 50.f);

    player_one_->DrawObject();

    player_two_->DrawObject();

    ball_->DrawObject();
}

void Game::ResetGame() {
    start_game_ = false;

    player_one_->SetDirectionX(static_cast<int>(DirectionPlayer::NOWHERE));
    player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));

    player_two_->SetDirectionX(static_cast<int>(DirectionPlayer::NOWHERE));
    player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));

    ball_->SetCollPlayerOne(false);
    ball_->SetCollPlayerTwo(false);
    ball_->SetOffset(0.f);

    if (player_one_->HasWinner()) {
        ball_->SetDirectionX(static_cast<int>(DirectionBall::RIGHT));
    }
    else if (player_two_->HasWinner()) {
        ball_->SetDirectionX(static_cast<int>(DirectionBall::LEFT));
    }
    
    player_one_->SetWinner(false);
    player_two_->SetWinner(false);

    ball_->SetDirectionY(static_cast<int>(DirectionBall::NOWHERE));

    glm::vec2 player(0.f, render_->GetWindowHeight() / 2);
    glm::vec2 ball (render_->GetWindowWidth() / 2 - BALL_SIZE.x, render_->GetWindowHeight() / 2 - BALL_SIZE.y);
    glm::vec2 player_two(render_->GetWindowWidth(), render_->GetWindowHeight() / 2);

    player_one_->SetPos(player);
    player_two_->SetPos(player_two);
    ball_->SetPos(ball);

    player_one_->DrawObject();
    player_two_->DrawObject();
    ball_->DrawObject();
}

glm::vec2 Game::GetPositionObj(GameObject* obj) {
    auto [obj_x, obj_y] = obj->GetPos();
    return {obj_x, obj_y};
}

glm::vec2 Game::GetSizeObj(GameObject* obj) {
    auto [obj_width, obj_height] = obj->GetSize();
    return {obj_width, obj_height};
}

void Game::UpdateDataLog() {
    std::vector<std::string> data_to_log = {
        std::to_string(player_one_->GetPos().y),
        std::to_string(player_two_->GetPos().y),
        std::to_string(ball_->GetPos().x),
        std::to_string(ball_->GetPos().y),
        std::to_string(ball_->GetVelocity().x),
        std::to_string(ball_->GetVelocity().y),
        std::to_string(ball_->GetDirectionX()),
        std::to_string(ball_->GetDirectionY()),
        std::to_string(dt_),
        std::to_string(ball_->GetRotate()),
        std::to_string(ball_->GetCollPlayerOne()),
        std::to_string(ball_->GetCollPlayerTwo())
    };

    Logger::UpdateDataLog(data_to_log);

    Logger::Log(render_, 0.f , 580.f, 0.2f, WHITE);
}
