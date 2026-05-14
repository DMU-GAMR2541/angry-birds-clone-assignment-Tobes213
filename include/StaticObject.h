#pragma once
#include "GameObject.h"

// Base class for objects that stay in fixed positions
class StaticObject : public GameObject {
protected:

    // Stores object position
    float f_posX;
    float f_posY;

public:
    StaticObject(float posX = 0.0f, float posY = 0.0f)
    : f_posX(posX), f_posY(posY) {}

    // Must be overridden in derived classes
    virtual void update() = 0;
    virtual void render() = 0;
    virtual std::string getType() const = 0;

    // Virtual destructor for proper inheritance cleanup
    virtual ~StaticObject() {
        std::cout << "StaticObject destroyed" << std::endl;
    }
};