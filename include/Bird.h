#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>

class Bird : public DynamicObject {
protected:
    float f_mass;
    float f_speed;
    std::string str_birdType;

    sf::Sprite sp_rendered;
    sf::Texture sf_tex;
    const float SCALE = 30.0f;

public:
    Bird(std::string type, float mass, float speed,
        std::string spritePath, sf::IntRect spriteRect,
        float posX = 0.0f, float posY = 0.0f)
        : DynamicObject(posX, posY), str_birdType(type), f_mass(mass), f_speed(speed)
    {
        if (!sf_tex.loadFromFile(spritePath))
            std::cout << "Failed to load bird texture: " << spritePath << std::endl;

        sp_rendered.setTexture(sf_tex);
        sp_rendered.setTextureRect(spriteRect);
        sp_rendered.setOrigin(spriteRect.width / 2.0f, spriteRect.height / 2.0f);
        sp_rendered.setPosition(posX, posY);
    }

    void update() override {
        std::cout << "Bird update: " << str_birdType << std::endl;
    }

    void render() override {
        std::cout << "Bird render: " << str_birdType << std::endl;
    }

    void render(sf::RenderWindow& window) {
        window.draw(sp_rendered);
    }

    std::string getType() const override { return "Bird: " + str_birdType; }

    float getMass() const { return f_mass; }
    float getSpeed() const { return f_speed; }

    virtual ~Bird() {
        std::cout << "Bird destroyed" << std::endl;
    }
};