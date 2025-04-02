#pragma once

#include <glad.h>

struct SizeObject {
    SizeObject(GLfloat x, GLfloat y) {
        this->x = x;
        this->y = y;
    }
    
    GLfloat x; 
    GLfloat y;
};

struct PositionObject {
    PositionObject(GLfloat x, GLfloat y) {
        this->x = x;
        this->y = y;
    }
    
    GLfloat x;
    GLfloat y;
};

// Игровой класс от которого наследуется шар, игроки или другие игровые классы объектов
class GameObject {
public:
    GameObject() = delete;

    GameObject(SizeObject sz, PositionObject pos) : size_(sz), position_(pos) {}

    GameObject(const GameObject&) = delete;

    inline SizeObject GetSize() const { return size_; }
    inline PositionObject GetPos() const { return position_; }
    
    inline void SetPosX(GLfloat x) { position_.x = x; }
    inline void SetPosY(GLfloat y) { position_.y = y; }

private:
    SizeObject size_;
    PositionObject position_;
};
