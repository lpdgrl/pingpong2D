#pragma once

#include "game_object.hpp"

// Игровой класс игрока наследуется от GameObject
class Player : public GameObject {
public:
    Player() { GameObject(); }
    Player(SizeObject sz, PositionObject pos, glm::vec2 velocity, int direction_x, int direction_y) : 
    GameObject(sz, pos, velocity, direction_x, direction_y) {}

    Player(const Player& object) = delete;
    Player& operator=(const Player& object) = delete;

    Player(Player&& object) = delete;
    Player& operator=(Player&& object) = delete;

    ~Player() noexcept {}

private:
};
