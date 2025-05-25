#pragma once

#include "game_object.hpp"

// Игровой класс шара наследуется от GameObject. Добавлены поля направление (direction) и была ли коллизия (wasColliding)
class Ball : public GameObject{
public:
    Ball() {
        GameObject();
    }

    Ball(SizeObject sz, PositionObject pos, glm::vec2 velocity, int direction_x, int direction_y, Render* render) : 
    GameObject(render, sz, pos, velocity, direction_x, direction_y) {}

    Ball(const Ball* object) = delete;
    Ball& operator=(const Ball& object) = delete;
    
    Ball(Ball&& object) = delete;
    Ball& operator=(Ball&& object) = delete;

    ~Ball() noexcept {}

    inline bool WasCollision() const { return was_collision_; }
    inline GLfloat GetOffset() const { return offset_; }
    inline GLfloat GetRotate() const { return rotate_; }

    inline void SetCollsion (bool collision) { was_collision_ = collision; }
    inline void SetOffset(GLfloat offset) { offset_ = offset; }
    inline void SetRotate(GLfloat rotate) { rotate_ = rotate; }

    inline void Move(GLfloat dt) override { 
        position_ += PositionObject(direction_x_ * (velocity_.x * dt * 5.f), direction_y_ * (velocity_.y * dt * 5.f));
        position_.x += offset_;
        position_.y += offset_;
    }

private:
    bool was_collision_ = false;
    GLfloat offset_ = 0.f;
    GLfloat rotate_ = 0.f;
};
