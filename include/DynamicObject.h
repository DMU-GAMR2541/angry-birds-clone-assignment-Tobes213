#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject {
protected:
    float f_posX;
    float f_posY;
    float f_velocityX;
    float f_velocityY;
    

public:
    DynamicObject(float posX = 0.0f, float posY = 0.0f)
        : f_posX(posX), f_posY(posY), f_velocityX(0.0f), f_velocityY(0.0f) {}

    virtual void update() = 0;
    virtual void render() = 0;
    virtual std::string getType() const = 0;

    virtual ~DynamicObject() {
        std::cout << "DynamicObject destroyed" << std::endl;
    }
};
