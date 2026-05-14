#pragma once
#include "GameObject.h"

// Base class for moving game objects
class DynamicObject : public GameObject {
protected:

    // Object position values
    float f_posX;
    float f_posY;

    // Object movement speed values
    float f_velocityX;
    float f_velocityY;
    

public:
    DynamicObject(float posX = 0.0f, float posY = 0.0f)
        : f_posX(posX), f_posY(posY), f_velocityX(0.0f), f_velocityY(0.0f) {}

    // Functions required in derived classes
    virtual void update() = 0;
    virtual void render() = 0;
    virtual std::string getType() const = 0;

    // Virtual destructor for correct cleanup
    virtual ~DynamicObject() {
        std::cout << "DynamicObject destroyed" << std::endl;
    }
};
