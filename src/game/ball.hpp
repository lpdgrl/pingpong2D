#pragma once

#include "game_object.hpp"

// Игровой класс шара наследуется от GameObject. Добавлены поля направление (direction) и была ли коллизия (wasColliding)
class Ball : public GameObject{
public:
    Ball() {
        GameObject();
        was_colliding_pl_one_ = false;
        was_colliding_pl_two_ = false;
    }

    Ball(SizeObject sz, PositionObject pos, glm::vec2 velocity, int direction_x, int direction_y, bool collding_pl_one, bool colding_pl_two, Render* render) : 
    GameObject(render, sz, pos, velocity, direction_x, direction_y),
    was_colliding_pl_one_(collding_pl_one), was_colliding_pl_two_(colding_pl_two) {}

    Ball(const Ball* object) = delete;
    Ball& operator=(const Ball& object) = delete;
    
    Ball(Ball&& object) = delete;
    Ball& operator=(Ball&& object) = delete;

    ~Ball() noexcept {}

    inline bool GetCollPlayerOne() const { return was_colliding_pl_one_; }
    inline bool GetCollPlayerTwo() const { return was_colliding_pl_two_; }
    inline GLfloat GetOffset() const { return offset_; }

    inline void SetCollPlayerOne(bool was_colliding) { was_colliding_pl_one_ = was_colliding; }
    inline void SetCollPlayerTwo(bool was_colliding) { was_colliding_pl_two_ = was_colliding; }
    inline void SetOffset(GLfloat offset) { offset_ = offset; }
private:
    bool was_colliding_pl_one_;
    bool was_colliding_pl_two_;
    GLfloat offset_ = 0.f;
};
