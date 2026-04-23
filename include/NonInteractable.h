#pragma once
#include "StaticObject.h"

class NonInteractable : public StaticObject {
private:
    std::string str_name;

public:
    NonInteractable(std::string name, float posX = 0.0f, float posY = 0.0f)
        : StaticObject(posX, posY), str_name(name) {
    }

    void update() override {
        std::cout << "NonInteractable update: " << str_name << std::endl;
    }

    void render() override {
        std::cout << "NonInteractable render: " << str_name << std::endl;
    }

    std::string getType() const override { return "NonInteractable"; }

    virtual ~NonInteractable() {
        std::cout << "NonInteractable destroyed" << std::endl;
    }
};