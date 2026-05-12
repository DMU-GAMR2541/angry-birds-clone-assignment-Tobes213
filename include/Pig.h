#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Pig : public DynamicObject {
protected:
    int i_health;
    float f_size;
    bool b_isDestroyed;

    sf::Sprite sp_rendered;
    sf::Texture sf_tex;

    b2Vec2 b2_pos;
    b2BodyDef b2_bodyDef;
    b2FixtureDef b2_fixtureDef;
    b2Body* b2_body;
    b2CircleShape b2_dynamicCircle;

    const float SCALE = 30.0f;

public:
    Pig(float size, int health, b2World& b2_world, b2Vec2 b2_posIn, sf::RenderWindow& sf_window, std::string str_SpriteLocation, sf::IntRect spriteRect = sf::IntRect(0, 0, 120, 120))
        : DynamicObject(b2_posIn.x, b2_posIn.y), f_size(size), i_health(health), b_isDestroyed(false) {

        if (!sf_tex.loadFromFile(str_SpriteLocation)) {
            std::cout << "Failed to load texture: " << str_SpriteLocation << std::endl;
        }

        sp_rendered.setTexture(sf_tex);

        float scale = size / (spriteRect.width / 2.0f);
        sp_rendered.setScale(scale, scale);

        sp_rendered.setTextureRect(spriteRect);
        sp_rendered.setOrigin(spriteRect.width / 2.0f, spriteRect.height / 2.0f);

        b2_bodyDef.type = b2_dynamicBody;
        b2_bodyDef.position = b2_posIn;
        b2_body = b2_world.CreateBody(&b2_bodyDef);

        b2_dynamicCircle.m_radius = size / SCALE;
        b2_fixtureDef.shape = &b2_dynamicCircle;
        b2_fixtureDef.density = 1.0f;
        b2_fixtureDef.friction = 0.3f;
        b2_fixtureDef.restitution = 0.5f;

        b2_body->CreateFixture(&b2_fixtureDef);
    }

    void update() override {
        sp_rendered.setPosition(
            b2_body->GetPosition().x * SCALE,
            b2_body->GetPosition().y * SCALE
        );
        sp_rendered.setRotation(b2_body->GetAngle() * (180.0f / 3.1415927f));
    }

    void render(sf::RenderWindow& sf_window) {
        sf_window.draw(sp_rendered);
    }

    void render() override {
        std::cout << "Pig render, size: " << f_size << std::endl;
    }

    void takeDamage(int damage) {
        i_health -= damage;
        if (i_health <= 0) {
            i_health = 0;
            b_isDestroyed = true;
        }
    }

    void applyImpulse(float x, float y) {
        b2_body->ApplyLinearImpulse(b2Vec2(x, y), b2_body->GetWorldCenter(), true);
    }

    b2Body* getBody() { return b2_body; }

    std::string getType() const override { return "Pig"; }
    bool isDestroyed() const { return b_isDestroyed; }

    virtual ~Pig() {
        std::cout << "Pig destroyed" << std::endl;
    }
};