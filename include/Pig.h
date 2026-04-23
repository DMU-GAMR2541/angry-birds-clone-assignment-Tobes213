#pragma once
#include "DynamicObject.h"

class Pig : public DynamicObject {
protected:
    int i_health;
    float f_size;
    bool b_isDestroyed;

public:
    Pig(float size, int health, float posX = 0.0f, float posY = 0.0f)
        : DynamicObject(posX, posY), f_size(size), i_health(health), b_isDestroyed(false) {
    }

    void takeDamage(int damage) {
        i_health -= damage;
        if (i_health <= 0) {
            i_health = 0;
            b_isDestroyed = true;
        }
    }

    void update() override {
        std::cout << "Pig update, health: " << i_health << std::endl;
    }

    void render() override {
        std::cout << "Pig render, size: " << f_size << std::endl;
    }

    std::string getType() const override { return "Pig"; }

    bool isDestroyed() const { return b_isDestroyed; }

    virtual ~Pig() {
        std::cout << "Pig destroyed" << std::endl;
    }
};