#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Bird : public DynamicObject {
protected:
    float f_mass;
    float f_speed;
    std::string str_birdType;

    sf::Sprite sp_rendered;
    sf::Texture sf_tex;
    const float SCALE = 30.0f;

    b2Body* b2_body = nullptr;
    b2BodyDef b2_bodyDef;
    b2CircleShape b2_circle;
    b2FixtureDef b2_fixtureDef;

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
        if (b2_body) {
            sp_rendered.setPosition(
                b2_body->GetPosition().x * SCALE,
                b2_body->GetPosition().y * SCALE
            );
            sp_rendered.setRotation(b2_body->GetAngle() * (180.0f / 3.1415927f));
        }
    }

    void initPhysics(b2World& world) {
        b2_bodyDef.type = b2_dynamicBody;
        b2_bodyDef.position.Set(sp_rendered.getPosition().x / SCALE, sp_rendered.getPosition().y / SCALE);
        b2_body = world.CreateBody(&b2_bodyDef);
        b2_circle.m_radius = 15.0f / SCALE;
        b2_fixtureDef.shape = &b2_circle;
        b2_fixtureDef.density = f_mass;
        b2_fixtureDef.restitution = 0.3f;
        b2_fixtureDef.friction = 0.5f;
        b2_body->CreateFixture(&b2_fixtureDef);
    }

    void fire(float forceX, float forceY) {
        if (b2_body)
            b2_body->ApplyLinearImpulse(b2Vec2(forceX, forceY), b2_body->GetWorldCenter(), true);
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