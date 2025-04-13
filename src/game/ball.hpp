#pragma once

#include "game_object.hpp"

// Игровой класс шара наследуется от GameObject. Добавлены поля направление (direction) и была ли коллизия (wasColliding)
class Ball : public GameObject{
public:
    Ball() {
        GameObject();
        was_colliding_ = false;
    }

    Ball(SizeObject sz, PositionObject pos, glm::vec2 velocity, int direction_x, int direction_y, bool wasColliding) : 
    GameObject(sz, pos, velocity, direction_x, direction_y),
    was_colliding_(wasColliding) {}

    Ball(const Ball* object) = delete;
    Ball& operator=(const Ball& object) = delete;
    
    Ball(Ball&& object) = delete;
    Ball& operator=(Ball&& object) = delete;

    ~Ball() noexcept {}

    inline bool GetWasColliding() const { return was_colliding_; }

    inline void SetWasColliding(bool was_colliding) { was_colliding_ = was_colliding; }

private:
    bool was_colliding_;
};
