#pragma once
#include <iostream>

class Enemy {
/// <summary>
/// Varaibles that define an enemy.
/// </summary>
private:
    int i_health;
    bool b_isDestroyed;
    float f_posX;
    float f_posY;

public:
    //Default constructor for an enemy. 
    Enemy() = default;
    Enemy(int i_initialHealth, float posX = 0.0f, float posY = 0.0f)
        : i_health(i_initialHealth), b_isDestroyed(false), f_posX(posX), f_posY(posY) {
    }

    //Class functions to be tested.
    void takeDamage(int damage) {
        if (damage < 0) return;
            i_health -= damage;
        if (i_health <= 0) {
            i_health = 0;
            b_isDestroyed = true;
        }
    }

    int getHealth() const { return i_health; }
    bool checkIfPopped() const { return b_isDestroyed; }

    void setHealth(int health) {
        i_health = health;
        std::cout << "Health set to: " << health << std::endl;
    }

    std::pair<float, float> getPosition() const {
        std::cout << "Position: (" << f_posX << ", " << f_posY << ")" << std::endl;
        return { f_posX, f_posY };
    }
};