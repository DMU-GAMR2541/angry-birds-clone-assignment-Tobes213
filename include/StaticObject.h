#pragma once
#include "GameObject.h"

class StaticObject : public GameObject {
protected:
    float f_posX;
    float f_posY;

public:
    StaticObject(float posX = 0.0f, float posY = 0.0f)
        : f_posX(posX), f_posY(posY) {}


    virtual void update() = 0;
    virtual void render() = 0;
    virtual std::string getType() const = 0;

    virtual ~StaticObject() {
        std::cout << "StaticObject destroyed" << std::endl;
    }
};