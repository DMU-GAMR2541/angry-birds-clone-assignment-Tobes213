#pragma once
#include "DynamicObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

class Bird : public DynamicObject {
protected:
    float f_mass;
    float f_speed;
    std::string str_birdType;
    bool b_activated = false;
    bool b_isExploded = false;
    bool b_showExplosion = false;
    float f_explosionRadius = 0.0f;
    sf::Vector2f v_explosionPos;
    b2Body* b2_egg = nullptr;
    sf::CircleShape sh_egg;
    bool b_eggDropped = false;
    bool b_eggExploded = false;
    sf::Vector2f v_eggExplosionPos;
    float f_eggExplosionRadius = 0.0f;
    bool b_showEggExplosion = false;
    b2World* b2_worldRef = nullptr;

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
        if (b2_egg && !b_eggExploded) {
            sf::Vector2f eggPos(b2_egg->GetPosition().x * SCALE, b2_egg->GetPosition().y * SCALE);
            sh_egg.setPosition(eggPos);
            b2Vec2 eggVel = b2_egg->GetLinearVelocity();
            float eggSpeed = std::sqrt(eggVel.x * eggVel.x + eggVel.y * eggVel.y);
            if (eggSpeed < 0.5f && b2_egg->GetPosition().y * SCALE > 200.0f) {
                b_eggExploded = true;
                b_showEggExplosion = true;
                v_eggExplosionPos = eggPos;
                float eggExplosionRadius = 2.0f;
                float eggExplosionForce = 25.0f;
                for (b2Body* body = b2_egg->GetWorld()->GetBodyList(); body; body = body->GetNext()) {
                    if (body == b2_egg) continue;
                    b2Vec2 dir = body->GetPosition() - b2_egg->GetPosition();
                    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                    if (dist < eggExplosionRadius && dist > 0.0f) {
                        b2Vec2 impulse(dir.x / dist * eggExplosionForce, dir.y / dist * eggExplosionForce);
                        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
                        if (body->GetUserData().pointer >= 3 && body->GetUserData().pointer <= 5)
                            body->GetUserData().pointer = 999;
                    }
                }
                b2_egg->SetType(b2_staticBody);
            }
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
        b2_worldRef = &world;
    }

    void fire(float forceX, float forceY) {
        if (b2_body)
            b2_body->ApplyLinearImpulse(b2Vec2(forceX, forceY), b2_body->GetWorldCenter(), true);
    }

    void activate(std::vector<b2Body*> pigBodies = {}) {
        if (!b2_body) return;

        if (str_birdType == "Chuck" && !b_activated) {
            b_activated = true;
            b2Vec2 vel = b2_body->GetLinearVelocity();
            float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
            b2Vec2 normalised(vel.x / speed, vel.y / speed);
            b2_body->SetLinearVelocity(b2Vec2(normalised.x * speed * 2.5f, normalised.y * speed * 2.5f));
            std::cout << "Chuck speed boost activated!" << std::endl;
        }
        else if (str_birdType == "Bomb" && !b_activated) {
            b_activated = true;
            b2Vec2 bombPos = b2_body->GetPosition();
            float explosionRadius = 3.5f;
            float explosionForce = 50.0f;
            for (b2Body* body = b2_body->GetWorld()->GetBodyList(); body; body = body->GetNext()) {
                if (body == b2_body) continue;
                b2Vec2 dir = body->GetPosition() - bombPos;
                float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (dist < explosionRadius && dist > 0.0f) {
                    b2Vec2 impulse(dir.x / dist * explosionForce, dir.y / dist * explosionForce);
                    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
                    if (body->GetUserData().pointer >= 3 && body->GetUserData().pointer <= 5)
                        body->GetUserData().pointer = 999;
                }
            }
            b_isExploded = true;
            b2_body->SetType(b2_staticBody);
            b2_body->SetLinearVelocity(b2Vec2(0, 0));
            sp_rendered.setPosition(-1000.0f, -1000.0f);
            b_showExplosion = true;
            v_explosionPos = sf::Vector2f(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
        }
        else if (str_birdType == "Matilda" && !b_eggDropped) {
            b_eggDropped = true;
            b2BodyDef eggDef;
            eggDef.type = b2_dynamicBody;
            eggDef.position = b2_body->GetPosition();
            b2_egg = b2_worldRef->CreateBody(&eggDef);
            b2_egg->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            b2CircleShape eggShape;
            eggShape.m_radius = 0.3f;
            b2FixtureDef eggFixture;
            eggFixture.shape = &eggShape;
            eggFixture.density = 1.0f;
            eggFixture.restitution = 0.0f;
            b2_egg->CreateFixture(&eggFixture);
            b2_egg->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            b2_egg->SetGravityScale(3.0f);
            b2_egg->SetFixedRotation(true);
            sh_egg.setRadius(8.0f);
            sh_egg.setOrigin(8.0f, 10.0f);
            sh_egg.setFillColor(sf::Color::White);
            sh_egg.setOutlineColor(sf::Color(200, 200, 200));
            sh_egg.setOutlineThickness(1.0f);
            sh_egg.setScale(0.75f, 1.0f);
            std::cout << "Matilda dropped egg!" << std::endl;
        }
    }

    void reset() {
        b_activated = false;
        b_eggDropped = false;
    }

    void setPosition(float x, float y) {
        if (b2_body)
            b2_body->SetTransform(b2Vec2(x / SCALE, y / SCALE), 0);
        sp_rendered.setPosition(x, y);
    }

    void setStatic(bool isStatic) {
        if (b2_body) {
            b2_body->SetType(isStatic ? b2_staticBody : b2_dynamicBody);
            b2_body->SetLinearVelocity(b2Vec2(0, 0));
            b2_body->SetAngularVelocity(0);
        }
    }

    void render() override {
        std::cout << "Bird render: " << str_birdType << std::endl;
    }

    void render(sf::RenderWindow& window) {
        if (!b_isExploded)
            window.draw(sp_rendered);

        if (b_showExplosion && f_explosionRadius < 120.0f) {
            f_explosionRadius += 5.0f;
            sf::CircleShape explosion(f_explosionRadius);
            explosion.setOrigin(f_explosionRadius, f_explosionRadius);
            explosion.setPosition(v_explosionPos);
            explosion.setFillColor(sf::Color(255, 100, 0, 150 - static_cast<int>(f_explosionRadius)));
            explosion.setOutlineColor(sf::Color(255, 50, 0, 200));
            explosion.setOutlineThickness(2.0f);
            window.draw(explosion);
        }
        else if (f_explosionRadius >= 120.0f) {
            b_showExplosion = false;
        }
        if (b_showEggExplosion && f_eggExplosionRadius < 80.0f) {
            f_eggExplosionRadius += 4.0f;
            sf::CircleShape eggExplosion(f_eggExplosionRadius);
            eggExplosion.setOrigin(f_eggExplosionRadius, f_eggExplosionRadius);
            eggExplosion.setPosition(v_eggExplosionPos);
            eggExplosion.setFillColor(sf::Color(255, 255, 255, 100 - static_cast<int>(f_eggExplosionRadius)));
            eggExplosion.setOutlineColor(sf::Color(200, 200, 200, 200));
            eggExplosion.setOutlineThickness(2.0f);
            window.draw(eggExplosion);
        }
        else if (f_eggExplosionRadius >= 80.0f) {
            b_showEggExplosion = false;
        }
        if (b2_egg && !b_eggExploded)
            window.draw(sh_egg);
    }

    std::string getType() const override { return "Bird: " + str_birdType; }

    float getMass() const { return f_mass; }
    float getSpeed() const { return f_speed; }
    bool hasPhysics() const { return b2_body != nullptr; }
    b2Body* getBody() { return b2_body; }

    virtual ~Bird() {
        std::cout << "Bird destroyed" << std::endl;
    }
};