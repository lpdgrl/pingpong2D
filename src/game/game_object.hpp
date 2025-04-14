#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct SizeObject {
    SizeObject() {
        x = 0;
        y = 0;
    }

    SizeObject(GLfloat w, GLfloat h) {
        this->x = w;
        this->y = h;
    }
    
    GLfloat x; 
    GLfloat y;
};

struct PositionObject {
    PositionObject() {
        x = 0;
        y = 0;
    }

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
    GameObject() {
        velocity_ = {0.f, 0.f};
        direction_x_ = 0;
        direction_y_ = 0;
    };

    GameObject(SizeObject sz, PositionObject pos, glm::vec2 velocity, int direction_x, int direction_y) : 
    size_(sz), 
    position_(pos), 
    velocity_(velocity), 
    direction_x_(direction_x),
    direction_y_(direction_y) {}

    GameObject(const GameObject& object) = delete;
    GameObject& operator=(const GameObject& object) = delete;

    GameObject(GameObject&& object) = delete;
    GameObject& operator=(GameObject&& object) = delete;

    ~GameObject() noexcept { }

    virtual SizeObject GetSize() const { return size_; }
    virtual PositionObject GetPos() const { return position_; }

    virtual glm::vec2 GetVelocity() const { return velocity_; }
    virtual glm::vec2 GetSizeVec() const {return {size_.x, size_.y}; }
    virtual glm::vec2 GetCoordVec() const {return {position_.x, position_.y}; }

    virtual int GetDirectionX() const { return direction_x_; }
    virtual int GetDirectionY() const { return direction_y_; }

    virtual void SetPos(glm::vec2 pos) { position_.x = pos.x; position_.y = pos.y; }
    virtual void SetPosX(GLfloat x) { position_.x = x; }
    virtual void SetPosY(GLfloat y) { position_.y = y; }

    virtual void SetSize(glm::vec2 size) { size_.x = size.x; size_.y = size.y; }
    virtual void SetSizeW(GLfloat width) { size_.x = width; }
    virtual void SetSizeH(GLfloat height) { size_.y = height; }

    virtual void SetDirectionX(int direction) { if (direction > 1 || direction < -1) { direction_x_ = 0; } direction_x_ = direction; }
    virtual void SetDirectionY(int direction) { if (direction > 1 || direction < -1) { direction_y_ = 0; } direction_y_ = direction; }


protected:
    SizeObject size_;
    PositionObject position_;
    glm::vec2 velocity_;
    int direction_x_;
    int direction_y_;
};
