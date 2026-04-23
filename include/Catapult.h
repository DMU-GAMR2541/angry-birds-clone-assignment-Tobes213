#pragma once
#include "DynamicObject.h"
#include "Bird.h"

class Catapult : public DynamicObject {
private:
    int i_tension;
    const int MAX_TENSION = 100;
    Bird* loadedBird;

public:
    Catapult(float posX = 0.0f, float posY = 0.0f)
        : DynamicObject(posX, posY), i_tension(0), loadedBird(nullptr) {
    }

    void loadBird(Bird* bird) {
        loadedBird = bird;
        std::cout << "Bird loaded: " << bird->getType() << std::endl;
    }

    void pullBack(int amount) {
        i_tension = (i_tension + amount > MAX_TENSION) ? MAX_TENSION : i_tension + amount;
        std::cout << "Tension: " << i_tension << std::endl;
    }

    void release() {
        if (loadedBird) {
            std::cout << "Firing " << loadedBird->getType() << " with tension " << i_tension << std::endl;
            i_tension = 0;
            loadedBird = nullptr;
        }
    }

    void update() override {
        std::cout << "Catapult update" << std::endl;
    }

    void render() override {
        std::cout << "Catapult render" << std::endl;
    }

    std::string getType() const override { return "Catapult"; }

    virtual ~Catapult() {
        std::cout << "Catapult destroyed" << std::endl;
    }
};