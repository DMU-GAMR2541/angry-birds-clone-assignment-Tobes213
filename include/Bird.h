#pragma once
#include "DynamicObject.h"

class Bird : public DynamicObject {
protected:
    float f_mass;
    float f_speed;
    std::string str_birdType;

public:
    Bird(std::string type, float mass, float speed, float posX = 0.0f, float posY = 0.0f)
        : DynamicObject(posX, posY), str_birdType(type), f_mass(mass), f_speed(speed) {
    }

    void update() override {
        std::cout << "Bird update: " << str_birdType << std::endl;
    }

    void render() override {
        std::cout << "Bird render: " << str_birdType << std::endl;
    }

    std::string getType() const override { return "Bird: " + str_birdType; }

    float getMass() const { return f_mass; }
    float getSpeed() const { return f_speed; }

    virtual ~Bird() {
        std::cout << "Bird destroyed" << std::endl;
    }
};