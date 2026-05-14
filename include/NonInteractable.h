#pragma once
#include "StaticObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

// Structure blocks affected by physics
class NonInteractable : public StaticObject {
private:
    std::string str_name;

    // Visual shape of the object
    sf::RectangleShape sh_shape;

    // Physics body used by Box2D
    b2Body* b2_body = nullptr;
    const float SCALE = 30.0f;

    // Object durability
    int i_health;
    bool b_isDestroyed = false;
public:
    NonInteractable(std::string name, float posX, float posY, float width, float height, sf::Color colour, b2World& world)
        : StaticObject(posX, posY), str_name(name) {

        // Stone blocks take more hits
        i_health = (colour == sf::Color(150, 150, 150)) ? 2 : 1;

        sh_shape.setSize(sf::Vector2f(width, height));
        sh_shape.setOrigin(width / 2.0f, height / 2.0f);
        sh_shape.setPosition(posX + width / 2.0f, posY + height / 2.0f);
        sh_shape.setFillColor(colour);

        // Creates Box2D body
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set((posX + width / 2.0f) / SCALE, (posY + height / 2.0f) / SCALE);
        bodyDef.linearDamping = 0.5f;
        bodyDef.angularDamping = 0.5f;
        b2_body = world.CreateBody(&bodyDef);

        // Creates collision box
        b2PolygonShape boxShape;
        boxShape.SetAsBox((width / 2.0f) / SCALE, (height / 2.0f) / SCALE);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.5f;
        fixtureDef.friction = 0.8f;
        fixtureDef.restitution = 0.0f;
        b2_body->CreateFixture(&fixtureDef);
    }

    // Reduces health when hit
    void onHit() {
        i_health--;

        // Disables block when destroyed
        if (i_health <= 0) {
            b_isDestroyed = true;
            b2_body->SetEnabled(false);
        }
    }

    bool isDestroyed() const { return b_isDestroyed; }
    b2Body* getBody() { return b2_body; }

    void update() override {

        // Syncs visual shape with physics body
        if (!b_isDestroyed) {
            sh_shape.setPosition(
                b2_body->GetPosition().x * SCALE,
                b2_body->GetPosition().y * SCALE
            );
            sh_shape.setRotation(b2_body->GetAngle() * (180.0f / 3.1415927f));
        }
    }

    void render() override {
        std::cout << "NonInteractable render: " << str_name << std::endl;
    }

    // Draws block to the game window
    void render(sf::RenderWindow& window) {
        if (!b_isDestroyed)
            window.draw(sh_shape);
    }

    std::string getType() const override { return "NonInteractable"; }

    // Destructor message for debugging
    virtual ~NonInteractable() {
        std::cout << "NonInteractable destroyed" << std::endl;
    }
};