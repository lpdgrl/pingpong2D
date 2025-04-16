#include "game.hpp"

void Game::Update(GLfloat dt) {
    KeyPress press = ProcessInput(render_->GetWindow());

    bool wasColliding = ball_->GetCollPlayerOne();
    bool wasCollPlayerTwo = ball_->GetCollPlayerTwo();

    int directionX_ball = ball_->GetDirectionX();
    int directionY_ball = ball_->GetDirectionY();

    GLfloat offset = ball_->GetOffset();

    if (press == KeyPress::SPACE) {
        start_game_ = true;
    }
    else if (press == KeyPress::R) {
        ResetGame();
    }
    
    glm::vec2 coord_player = player_one_->GetCoordVec();
    glm::vec2 size_player = player_one_->GetSizeVec();

    glm::vec2 coord_player_two = player_two_->GetCoordVec();
    glm::vec2 size_player_two = player_two_->GetSizeVec();

    glm::vec2 coord_ball = ball_->GetCoordVec();
    glm::vec2 size_ball = ball_->GetSizeVec();

    // Перезапуск игры если мяч вышел за пределы
    if (coord_ball.x + size_ball.x < 0.f ) {
        player_two_->SetWinner();
        ResetGame();
    }
    else if (coord_ball.x + size_ball.x > render_->GetWindowWidth()) {
        player_one_->SetWinner();
        ResetGame();
    }
    
    if (start_game_) {
        glm::vec2 mv_pos_player;
        glm::vec2 mv_pos_player_two;
        glm::vec2 move_ball;
        
        // обрабатываем кнопки движения игрока (возможно стоит переписывать в метод move для игрового объекта типа игрок)
        switch(press) {
            case KeyPress::W: {
                if ((coord_player.y + size_player.y) <= render_->GetWindowHeight()) {
                    player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::UP));
                    mv_pos_player.y += player_one_->GetDirectionY() * player_one_->GetVelocity().y * dt;
                }
                break;
            }

            case KeyPress::S: {
                if ((coord_player.y - size_player.y) >= 0) {
                    player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::DOWN));
                    mv_pos_player.y += player_one_->GetDirectionY() * player_one_->GetVelocity().y * dt;
                }
                break;
            }

            case KeyPress::I: {
                if ((coord_player_two.y + size_player_two.y) <= render_->GetWindowHeight()) {
                    player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::UP));
                    mv_pos_player_two.y += player_two_->GetDirectionY() * player_two_->GetVelocity().y * dt;
                }
                break;
            }

            case KeyPress::K: {
                if ((coord_player_two.y - size_player_two.y) >= 0) {
                    player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::DOWN));
                    mv_pos_player_two.y += player_two_->GetDirectionY() * player_two_->GetVelocity().y * dt;
                }
                break;
            }

            default: {
                player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));
                player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));
                break;
            }
        }

        // чекаем коллизию
        bool collision_player_one = CheckCollision(player_one_, ball_, false);
        bool collision_player_two = CheckCollision(player_two_, ball_, true);

        std::cout << "Collision Player One: " << (collision_player_two) << std::endl;
        std::cout << "Collision Player Two: " << (collision_player_two) << std::endl;

        // проверяем была ли коллизия (переписать в метод)
        if (collision_player_one && !wasColliding) {
           directionX_ball *= -1;
           ball_->SetDirectionX(directionX_ball);
           ball_->SetDirectionY(player_one_->GetDirectionY());
        }
        
        if (collision_player_two && !wasCollPlayerTwo) {
            directionX_ball *= -1;
            ball_->SetDirectionX(directionX_ball);
            ball_->SetDirectionY(player_two_->GetDirectionY());
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

        // Координаты куда нужно сдвинуть мяч
        move_ball.x += ball_->GetDirectionX() * (ball_->GetVelocity().x * dt * 5.f);
        move_ball.y += ball_->GetDirectionY() * (ball_->GetVelocity().y * dt * 5.f);
        
        // Устанавливаем перед отрисокой новые координаты в объектах мяча и игрока
        ball_->SetPosX(coord_ball.x + move_ball.x);
        ball_->SetPosY(coord_ball.y + move_ball.y);

        player_one_->SetPosY(coord_player.y + mv_pos_player.y);
        player_two_->SetPosY(coord_player_two.y + mv_pos_player_two.y);
    }
    
    // Отрисовываем игровые объекты
    player_one_->DrawObject();

    player_two_->DrawObject();
    
    GLfloat rotate_ball = glfwGetTime();
    rotate_ball = std::sin(rotate_ball) * 200.f;

    ball_->DrawObject(AxisRotate::AXIS_Z, rotate_ball);

    GameLog(rotate_ball);
}

bool Game::CheckCollision(GameObject* one, GameObject* two, bool second_player) {
    glm::vec2 coord_one = one->GetCoordVec();
    glm::vec2 size_one = one->GetSizeVec();

    glm::vec2 coord_two = two->GetCoordVec();
    glm::vec2 size_two = two->GetSizeVec();

    bool collisionX = false;
    if (!second_player) {
        collisionX = coord_two.x - size_two.x <= coord_one.x + size_one.x;
        std::cout << "CollisionX One: " << collisionX << std::endl;
    }
    else {
        collisionX = coord_two.x + size_two.x >= coord_one.x - size_one.x;
        std::cout << "CollisionX Two: " << collisionX << std::endl;
    }

    // разобраться как убрать голые значения - используются для того, чтобы учитывать вверхний и нижний игрока
    bool collisionY = coord_two.y + size_two.y <= coord_one.y + size_one.y + 15.f && coord_two.y - size_two.y >= coord_one.y - size_one.y - 15.f;
    std::cout << "CollisionY: " << collisionY << std::endl;

    return collisionX && collisionY;
}

void Game::StartGame() {
    render_->SetOrthoProjection(0.f, 0.f, 0.f, 0.f, -50.f, 50.f);

    player_one_->DrawObject();

    player_two_->DrawObject();

    ball_->DrawObject();
}

void Game::InitGame() {
    render_->InitWindow();
    render_->InitRender();
}

void Game::ResetGame() {
    start_game_ = false;

    player_one_->SetDirectionX(static_cast<int>(DirectionPlayer::NOWHERE));
    player_one_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));

    player_two_->SetDirectionX(static_cast<int>(DirectionPlayer::NOWHERE));
    player_two_->SetDirectionY(static_cast<int>(DirectionPlayer::NOWHERE));

    ball_->SetCollPlayerOne(false);
    ball_->SetCollPlayerTwo(false);

    if (player_one_->HasWinner()) {
        ball_->SetDirectionX(static_cast<int>(DirectionBall::RIGHT));
    }
    else if (player_two_->HasWinner()) {
        ball_->SetDirectionX(static_cast<int>(DirectionBall::LEFT));
    }
    
    player_one_->SetWinner();
    player_two_->SetWinner();

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
    else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        return KeyPress::I;
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        return KeyPress::K;
    }

    return KeyPress::NONE;
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

    std::cout << "Position y-axis of player: " << player_one_->GetPos().y << std::endl;
    std::cout << "Velocity y-axis of player: " << player_one_->GetVelocity().y << std::endl;

    std::cout << "Position y-axis of ball: " << ball_->GetPos().y << std::endl;
    std::cout << "Velocity y-axis of ball: " << ball_->GetVelocity().y << std::endl;

    std::cout << "Position + Size of player: " << player_one_->GetPos().x + player_one_->GetSize().x
    << " Position x-axis + Size of ball: " << ball_->GetPos().x + player_one_->GetSize().x << std::endl;

    std::cout << "Rotate of ball: " << parametr << std::endl;
}