#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>

class StartScreen {
private:
    sf::RenderWindow& window;
    sf::Texture tex_background;
    sf::Texture tex_logo;
    sf::Sprite sp_background;
    sf::Sprite sp_logo;
    sf::Font font;
    sf::Text txt_title;
    sf::Text txt_loading;
    sf::Text txt_percentage;
    sf::RectangleShape sh_barBackground;
    sf::RectangleShape sh_barFill;

    std::mutex mtx_progress;
    std::atomic<int> i_spriteProgress{ 0 };
    std::atomic<int> i_physicsProgress{ 0 };
    bool b_done = false;

    void loadSprites() {
        for (int i = 0; i <= 50; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            std::lock_guard<std::mutex> lock(mtx_progress);
            i_spriteProgress = i;
            std::cout << "Sprite loading: " << i * 2 << "%" << std::endl;
        }
    }

    void loadPhysics() {
        for (int i = 0; i <= 50; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(60));
            std::lock_guard<std::mutex> lock(mtx_progress);
            i_physicsProgress = i;
            std::cout << "Physics loading: " << i * 2 << "%" << std::endl;
        }
    }

    std::future<int> getProgressAsync() {
        return std::async(std::launch::async, [this]() {
            int spriteVal, physicsVal;
            {
                std::lock_guard<std::mutex> lock(mtx_progress);
                spriteVal = i_spriteProgress;
                physicsVal = i_physicsProgress;
            }
            return (spriteVal + physicsVal);
            });
    }

public:
    StartScreen(sf::RenderWindow& win) : window(win) {
        if (!tex_background.loadFromFile("../assets/Ang_Birds/Sky.png"))
            std::cout << "Failed to load background" << std::endl;
        sp_background.setTexture(tex_background);
        sp_background.setScale(
            800.0f / tex_background.getSize().x,
            600.0f / tex_background.getSize().y
        );

        if (!tex_logo.loadFromFile("../assets/Ang_Birds/CrackBird.jpg"))
            std::cout << "Failed to load logo" << std::endl;
        sp_logo.setTexture(tex_logo);
        sp_logo.setScale(
            600.0f / tex_logo.getSize().x,
            300.0f / tex_logo.getSize().y
        );
        sp_logo.setPosition(100.0f, 50.0f);

        if (!font.loadFromFile("../assets/fonts/angry-birds.ttf"))
            std::cout << "Failed to load font" << std::endl;

        txt_title.setFont(font);
        txt_title.setString("Annoyed Flocks");
        txt_title.setCharacterSize(48);
        txt_title.setFillColor(sf::Color::White);
        txt_title.setOutlineColor(sf::Color::Black);
        txt_title.setOutlineThickness(3.0f);
        txt_title.setPosition(220.0f, 370.0f);

        txt_loading.setFont(font);
        txt_loading.setString("Loading...");
        txt_loading.setCharacterSize(24);
        txt_loading.setFillColor(sf::Color::White);
        txt_loading.setPosition(50.0f, 490.0f);

        txt_percentage.setFont(font);
        txt_percentage.setString("0%");
        txt_percentage.setCharacterSize(24);
        txt_percentage.setFillColor(sf::Color::White);
        txt_percentage.setPosition(700.0f, 490.0f);

        sh_barBackground.setSize(sf::Vector2f(700.0f, 30.0f));
        sh_barBackground.setPosition(50.0f, 530.0f);
        sh_barBackground.setFillColor(sf::Color(50, 50, 50, 200));
        sh_barBackground.setOutlineColor(sf::Color::White);
        sh_barBackground.setOutlineThickness(2.0f);

        sh_barFill.setSize(sf::Vector2f(0.0f, 30.0f));
        sh_barFill.setPosition(50.0f, 530.0f);
        sh_barFill.setFillColor(sf::Color(255, 165, 0));
    }

    void run() {
        std::thread t_sprites(&StartScreen::loadSprites, this);
        std::thread t_physics(&StartScreen::loadPhysics, this);

        bool b_exit = false;

        while (window.isOpen() && !b_exit) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (b_done && event.type == sf::Event::KeyPressed)
                    b_exit = true;
                if (b_done && event.type == sf::Event::MouseButtonPressed)
                    b_exit = true;
            }

            auto progressFuture = getProgressAsync();
            int totalProgress = progressFuture.get();
            int percentage = std::min(totalProgress, 100);

            sh_barFill.setSize(sf::Vector2f(700.0f * percentage / 100.0f, 30.0f));
            txt_percentage.setString(std::to_string(percentage) + "%");

            if (percentage >= 100) {
                b_done = true;
                txt_loading.setString("Press any key to start!");
            }

            window.clear();
            window.draw(sp_background);
            window.draw(sp_logo);
            window.draw(txt_title);
            window.draw(txt_loading);
            window.draw(sh_barBackground);
            window.draw(sh_barFill);
            window.draw(txt_percentage);
            window.display();
        }

        t_sprites.join();
        t_physics.join();
        std::cout << "Loading complete, starting game..." << std::endl;
    }
};