#include "game.hpp"

void Game::Update(GLfloat dt) {
    KeyPress press = ProcessInput(render_->GetWindow());

    bool wasColliding = ball_->GetWasColliding();
    int directionX_ball = ball_->GetDirectionX();
    int directionY_ball = ball_->GetDirectionY();

    if (press == KeyPress::SPACE) {
        start_game_ = true;
    }
    else if (press == KeyPress::R) {
        ResetGame();
    }
    
    glm::vec2 coord_player = player_->GetCoordVec();
    glm::vec2 size_player = player_->GetSizeVec();

    glm::vec2 coord_ball = ball_->GetCoordVec();
    glm::vec2 size_ball = ball_->GetSizeVec();

    // Перезапуск игры если мяч вышел за пределы
    if (coord_ball.x + size_ball.x < 0.f || coord_ball.x + size_ball.x > render_->GetWindowWidth() ) {
        ResetGame();
    }

    if (start_game_) {
        glm::vec2 mv_pos_player;
        glm::vec2 move_ball;
        
        // обрабатываем кнопки движения игрока
        switch(press) {
            case KeyPress::W: {
                if ((coord_player.y + size_player.y) <= render_->GetWindowHeight()) {
                    player_->SetDirectionY(static_cast<int>(DirectionPlayer::UP));
                    mv_pos_player.y += player_->GetDirectionY() * player_->GetVelocity().y * dt;
                }
                break;
            }

            case KeyPress::S: {
                if ((coord_player.y - size_player.y) >= 0) {
                    player_->SetDirectionY(static_cast<int>(DirectionPlayer::DOWN));
                    mv_pos_player.y += player_->GetDirectionY() * player_->GetVelocity().y * dt;
                }
                break;
            }

            default: {
                player_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));
                break;
            }
        }

        // чекаем коллизию
        bool collision = CheckColision(player_, ball_);
        std::cout << "Collision: " << collision << std::endl;
        
        // проверяем была ли коллизия
        if (collision && !wasColliding) {
           directionX_ball *= -1;
           ball_->SetDirectionX(directionX_ball);
           ball_->SetDirectionY(player_->GetDirectionY());
        }

        // устанавливаем состояние проверки коллизии
        ball_->SetWasColliding(collision); 

        // чекаем на совпадение границ экрана с координатами мяча, если совпадает то меняем направление
        if (coord_ball.y + size_ball.y >= render_->GetWindowHeight()) {
            ball_->SetDirectionY(static_cast<int>(DirectionBall::LEFT));
        } 
        else if (coord_ball.y + size_ball.y <= 0) {
            ball_->SetDirectionY(static_cast<int>(DirectionBall::RIGHT));
        }

        // Координаты куда нужно сдвинуть мяч
        move_ball.x += directionX_ball * (ball_->GetVelocity().x * dt * 5.f);
        move_ball.y += directionY_ball * (ball_->GetVelocity().y * dt * 10.f);
        
        // Устанавливаем перед отрисокой новые координаты в объектах мяча и игрока
        ball_->SetPosX(coord_ball.x + move_ball.x);
        ball_->SetPosY(coord_ball.y + move_ball.y);
        player_->SetPosY(coord_player.y + mv_pos_player.y);
    }
    
    glm::vec2 player_size = GetSizeObj(player_);
    glm::vec2 player_pos = GetPositionObj(player_);

    Draw(player_pos, player_size, AxisRotate::NONE, 0.f);

    glm::vec2 ball_pos = GetPositionObj(ball_);
    glm::vec2 ball_size = GetSizeObj(ball_);
    
    GLfloat rotate_ball = glfwGetTime();
    rotate_ball = !start_game_ ? std::sin(rotate_ball) * 200.f : 0.f;

    GameLog(rotate_ball);
    Draw(ball_pos, ball_size, AxisRotate::AXIS_Z, rotate_ball);
}

bool Game::CheckColision(GameObject* one, GameObject* two) {
    glm::vec2 coord_one = one->GetCoordVec();
    glm::vec2 size_one = one->GetSizeVec();

    glm::vec2 coord_two = two->GetCoordVec();
    glm::vec2 size_two = two->GetSizeVec();

    bool collisionX = coord_two.x - size_two.x <= coord_one.x + size_one.x;
    std::cout << "CollisionX: " << collisionX << std::endl;

    // разобраться как убрать голые значения - используются для того, чтобы учитывать вверхний и нижний игрока
    bool collisionY = coord_two.y + size_two.y <= coord_one.y + size_one.y + 15.f && coord_two.y - size_two.y >= coord_one.y - size_one.y - 15.f;
    std::cout << "CollisionY: " << collisionY << std::endl;

    return collisionX && collisionY;
}

void Game::StartGame() {
    render_->SetOrthoProjection(0.f, 0.f, 0.f, 0.f, -50.f, 50.f);

    glm::vec2 player_size = GetSizeObj(player_);
    glm::vec2 player_pos = GetPositionObj(player_);

    Draw(player_pos, player_size, AxisRotate::NONE, 0.f);

    glm::vec2 ball_pos = GetPositionObj(ball_);
    glm::vec2 ball_size = GetSizeObj(ball_);
    
    Draw(ball_pos, ball_size, AxisRotate::NONE, 0.f);
}

void Game::InitGame() {
    render_->InitWindow();
    render_->InitRender();
}

void Game::ResetGame() {
    start_game_ = false;

    player_->SetDirectionX(static_cast<int>(DirectionPlayer::NOWHERE));
    player_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));

    ball_->SetWasColliding(false);
    ball_->SetDirectionX(static_cast<int>(DirectionBall::LEFT));
    ball_->SetDirectionY(static_cast<int>(DirectionBall::NOWHERE));

    glm::vec2 player(0.f, render_->GetWindowHeight() / 2 - PLAYER_SIZE.y / 2);
    glm::vec2 ball (render_->GetWindowWidth() / 2 - BALL_SIZE.x, render_->GetWindowHeight() / 2 - BALL_SIZE.y);

    player_->SetPos(player);
    ball_->SetPos(ball);

    Draw(GetPositionObj(ball_), GetSizeObj(ball_), AxisRotate::NONE, 0.f);
    Draw(GetPositionObj(ball_), GetSizeObj(ball_), AxisRotate::NONE, 0.f);
}

void Game::Draw(const glm::vec2& position, const glm::vec2& size, AxisRotate axis, GLfloat rotate) {
    render_->Draw(position, size, axis, rotate);
}

KeyPress Game::ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        return KeyPress::W;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        return KeyPress::S;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        return KeyPress::SPACE;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        return KeyPress::R;
    }

    return KeyPress::None;
}

glm::vec2 Game::GetPositionObj(GameObject* obj) {
    auto [obj_x, obj_y] = obj->GetPos();
    return {obj_x, obj_y};
}

glm::vec2 Game::GetSizeObj(GameObject* obj) {
    auto [obj_width, obj_height] = obj->GetSize();
    return {obj_width, obj_height};
}

void Game::GameLog(GLfloat parametr) const {
    std::cout << std::boolalpha;

    std::cout << "Position y-axis of player: " << player_->GetPos().y << std::endl;
    std::cout << "Velocity y-axis of player: " << player_->GetVelocity().y << std::endl;

    std::cout << "Position y-axis of ball: " << ball_->GetPos().y << std::endl;
    std::cout << "Velocity y-axis of ball: " << ball_->GetVelocity().y << std::endl;

    std::cout << "Position + Size of player: " << player_->GetPos().x + player_->GetSize().x
    << " Position x-axis + Size of ball: " << ball_->GetPos().x + player_->GetSize().x << std::endl;

    std::cout << "Rotate of ball: " << parametr << std::endl;
}