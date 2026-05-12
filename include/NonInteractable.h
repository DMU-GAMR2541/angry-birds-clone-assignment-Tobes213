#pragma once
#include "StaticObject.h"
#include <SFML/Graphics.hpp>

class NonInteractable : public StaticObject {
private:
    std::string str_name;
    sf::RectangleShape sh_shape;

public:
    NonInteractable(std::string name, float posX, float posY, float width, float height, sf::Color colour)
        : StaticObject(posX, posY), str_name(name) {
        sh_shape.setSize(sf::Vector2f(width, height));
        sh_shape.setPosition(posX, posY);
        sh_shape.setFillColor(colour);
    }

    void update() override {}

    void render() override {
        std::cout << "NonInteractable render: " << str_name << std::endl;
    }

    void render(sf::RenderWindow& window) {
        window.draw(sh_shape);
    }

    std::string getType() const override { return "NonInteractable"; }

    virtual ~NonInteractable() {
        std::cout << "NonInteractable destroyed" << std::endl;
    }
};