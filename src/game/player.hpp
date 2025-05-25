#pragma once

#include "game_object.hpp"

// Игровой класс игрока наследуется от GameObject
class Player : public GameObject {
public:
    Player() { GameObject(); }
    Player(SizeObject sz, PositionObject pos, glm::vec2 velocity, int direction_x, int direction_y, Render* render) : 
    GameObject(render, sz, pos, velocity, direction_x, direction_y) {}

    Player(const Player& object) = delete;
    Player& operator=(const Player& object) = delete;

    Player(Player&& object) = delete;
    Player& operator=(Player&& object) = delete;

    ~Player() noexcept {}

    inline bool HasWinner() const { return last_winner_; }
    inline unsigned int GetScore() const { return score_; }
    inline void SetWinner(bool winner) { if (winner) { ++score_; } last_winner_ = winner; }
    inline void SetCollision(bool collision) { was_collising_ = collision; }
    inline bool WasCollision() { return was_collising_; }
    
    inline void Move(GLfloat dt) override { this->position_.y = this->position_.y + (this->direction_y_ * this->velocity_.y * dt); }

private:
    bool was_collising_ = false; 
    bool last_winner_ = false;
    unsigned int score_ = 0;
};
