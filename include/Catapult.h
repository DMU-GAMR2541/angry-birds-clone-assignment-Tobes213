#pragma once
#include "DynamicObject.h"
#include "Bird.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Catapult : public DynamicObject {
private:
    const float SCALE = 30.0f;
    float f_speedMultiplier;
    sf::RectangleShape sh_pole;
    sf::RectangleShape sh_leftArm;
    sf::RectangleShape sh_rightArm;
    sf::Vector2f v_launchPos;
    sf::Vector2f v_forkRestPos;
    bool b_isDragging = false;
    sf::Vector2f v_dragPos;
    Bird* loadedBird = nullptr;
    b2World* b2_world = nullptr;
    bool b_isFired = false;

public:
    Catapult(float posX, float posY, b2World& world, float speedMultiplier = 1.0f)
        : DynamicObject(posX, posY), f_speedMultiplier(speedMultiplier), b2_world(&world) {
        v_forkRestPos = sf::Vector2f(posX + 5.0f, posY - 70.0f);
        v_launchPos = v_forkRestPos;
        sf::Color woodColour(101, 67, 33);
        sh_pole.setSize(sf::Vector2f(10.0f, 80.0f));
        sh_pole.setOrigin(5.0f, 80.0f);
        sh_pole.setPosition(posX, posY);
        sh_pole.setFillColor(woodColour);
        sh_leftArm.setSize(sf::Vector2f(8.0f, 50.0f));
        sh_leftArm.setOrigin(4.0f, 50.0f);
        sh_leftArm.setPosition(posX, posY - 80.0f);
        sh_leftArm.setRotation(-30.0f);
        sh_leftArm.setFillColor(woodColour);
        sh_rightArm.setSize(sf::Vector2f(8.0f, 50.0f));
        sh_rightArm.setOrigin(4.0f, 50.0f);
        sh_rightArm.setPosition(posX, posY - 80.0f);
        sh_rightArm.setRotation(30.0f);
        sh_rightArm.setFillColor(woodColour);
    }

    bool isFired() const { return b_isFired; }
    Bird* getLoadedBird() { return loadedBird; }

    void loadBird(Bird* bird) {
    loadedBird = bird;
    if (bird && b2_world) {
        if (!bird->hasPhysics())
            bird->initPhysics(*b2_world);
        bird->getBody()->GetUserData().pointer = 100;
        bird->setStatic(true);
        bird->setPosition(v_forkRestPos.x, v_forkRestPos.y);
        std::cout << "Bird loaded: " << bird->getType() << std::endl;
        bird->reset();
    }
}

    void handleMousePress(sf::Vector2f mousePos) {
        if (b_isFired) return;
        float dist = std::sqrt(std::pow(mousePos.x - v_launchPos.x, 2) +
            std::pow(mousePos.y - v_launchPos.y, 2));
        if (dist < 60.0f) {
            b_isDragging = true;
            v_dragPos = mousePos;
        }
    }

    void handleMouseMove(sf::Vector2f mousePos) {
        if (b_isDragging) {
            float dx = mousePos.x - v_launchPos.x;
            float dy = mousePos.y - v_launchPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            float maxDrag = 80.0f;
            if (dist > maxDrag) {
                float ratio = maxDrag / dist;
                mousePos.x = v_launchPos.x + dx * ratio;
                mousePos.y = v_launchPos.y + dy * ratio;
            }
            v_dragPos = mousePos;
            if (loadedBird)
                loadedBird->setPosition(mousePos.x, mousePos.y);
        }
    }

    void handleMouseRelease(sf::Vector2f mousePos) {
        if (b_isDragging && loadedBird) {
            loadedBird->setStatic(false);
            float forceX = (v_launchPos.x - mousePos.x) * f_speedMultiplier * loadedBird->getSpeed() * 0.01f;
            float forceY = (v_launchPos.y - mousePos.y) * f_speedMultiplier * loadedBird->getSpeed() * 0.01f;
            std::cout << "Force: " << forceX << ", " << forceY << std::endl;
            loadedBird->fire(forceX, forceY);
            b_isFired = true;
        }
        b_isDragging = false;
    }

    void resetFired() {
        b_isFired = false;
        loadedBird = nullptr;
    }

    void update() override {
        if (!b_isDragging && !b_isFired && loadedBird)
            loadedBird->setPosition(v_forkRestPos.x, v_forkRestPos.y);
    }

    void render(sf::RenderWindow& window) {
        window.draw(sh_pole);
        window.draw(sh_leftArm);
        window.draw(sh_rightArm);
        if (loadedBird)
            loadedBird->render(window);
        if (b_isDragging && loadedBird) {
            float forceX = (v_launchPos.x - v_dragPos.x) * f_speedMultiplier * loadedBird->getSpeed() * 0.01f;
            float forceY = (v_launchPos.y - v_dragPos.y) * f_speedMultiplier * loadedBird->getSpeed() * 0.01f;
            float vx = forceX / loadedBird->getMass() * 0.5f;
            float vy = forceY / loadedBird->getMass() * 0.5f;
            float x = v_dragPos.x;
            float y = v_dragPos.y;
            float gravity = 0.163f;
            for (int i = 0; i < 30; i++) {
                vx *= 0.99f;
                vy += gravity * (1.0f / 60.0f);
                x += vx;
                y += vy;
                if (i % 2 == 0) {
                    sf::CircleShape dot(3.0f);
                    dot.setFillColor(sf::Color(255, 255, 255, 150));
                    dot.setOrigin(3.0f, 3.0f);
                    dot.setPosition(x, y);
                    window.draw(dot);
                }
            }
        }
    }

    void render() override {
        std::cout << "Catapult render" << std::endl;
    }

    std::string getType() const override { return "Catapult"; }

    virtual ~Catapult() {
        std::cout << "Catapult destroyed" << std::endl;
    }
};