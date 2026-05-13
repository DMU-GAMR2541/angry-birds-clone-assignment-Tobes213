#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h"
#include "Pig.h"
#include "Enemy.h"
#include "Catapult.h"
#include "DynamicObject.h"
#include "ContactListener.h"
#include "NonInteractable.h"
#include <list>
#include <array>
#include <tuple>
#include <map>

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    //Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    //Can set a definition for PI.catapult.update();
    const float PI = 3.1415927;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    ContactListener contactListener;
    world.SetContactListener(&contactListener);

    Pig pig1(20.0f, 100, world, b2Vec2(355.0f / 30.0f, 480.0f / 30.0f), "../assets/Ang_Birds/sprite_1.png", sf::IntRect(4, 5, 56, 47));
    Pig pig2(30.0f, 150, world, b2Vec2(355.0f / 30.0f, 340.0f / 30.0f), "../assets/Ang_Birds/sprite_2.png", sf::IntRect(5, 0, 89, 100));
    Pig pig3(40.0f, 200, world, b2Vec2(615.0f / 30.0f, 480.0f / 30.0f), "../assets/Ang_Birds/sprite_4.png", sf::IntRect(2, 8, 103, 98));

    pig1.getBody()->GetUserData().pointer = 3;
    pig2.getBody()->GetUserData().pointer = 4;
    pig3.getBody()->GetUserData().pointer = 5;

    std::list<Bird*> birdQueue;

    std::vector<std::tuple<std::string, float, float, std::string, sf::IntRect, float, float>> birdData = {
        {"Red",     1.0f, 10.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(906,797,45,51),   30.0f, 560.0f},
        {"Chuck",   1.0f, 10.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(667,879,61,63),   70.0f, 560.0f},
        {"Bomb",    1.0f, 10.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(408,726,65,80),   110.0f, 560.0f},
        {"Matilda", 1.0f, 10.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(418,638,73,85),   150.0f, 560.0f}
    };

    for (auto& [type, mass, speed, path, rect, x, y] : birdData) {
        Bird* b = new Bird(type, mass, speed, path, rect, x, y);
        birdQueue.push_back(b);
    }

    Catapult catapult(80.0f, 570.0f, world, 1.0f);
    if (!birdQueue.empty()) {
        catapult.loadBird(birdQueue.front());
        birdQueue.pop_front();
    }

    std::multimap<std::string, DynamicObject*> gameObjects;

    gameObjects.insert({ "pig", &pig1 });
    gameObjects.insert({ "pig", &pig2 });
    gameObjects.insert({ "pig", &pig3 });

    for (auto& [type, mass, speed, path, rect, x, y] : birdData) {
        gameObjects.insert({ "bird", birdQueue.back() });
    }

    auto pigs = gameObjects.equal_range("pig");
    for (auto it = pigs.first; it != pigs.second; ++it) {
        it->second->update();
    }

    //Setup ground for the circle to move / bounce on.
    //Needs to have a body definition and a body. We use a raw pointer for the b2Body as Box2d does the management itself.
    //A body can be defined as having a position, velocity, and mass. 
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    //Define a fixture shape that relates to the collision for the ground.
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    //Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    std::array<NonInteractable, 14> staticObjects = { {
     NonInteractable("Base1", 300.0f, 540.0f, 120.0f, 15.0f, sf::Color(139,90,43), world),
     NonInteractable("PostL1", 300.0f, 480.0f, 15.0f, 60.0f, sf::Color(139,90,43), world),
     NonInteractable("PostR1", 405.0f, 480.0f, 15.0f, 60.0f, sf::Color(139,90,43), world),
     NonInteractable("Shelf1", 300.0f, 480.0f, 120.0f, 15.0f, sf::Color(139,90,43), world),
     NonInteractable("PostL2", 300.0f, 390.0f, 15.0f, 90.0f, sf::Color(139,90,43), world),
     NonInteractable("PostR2", 405.0f, 390.0f, 15.0f, 90.0f, sf::Color(139,90,43), world),
     NonInteractable("Shelf2", 300.0f, 390.0f, 120.0f, 15.0f, sf::Color(139,90,43), world),
     NonInteractable("Stone1", 300.0f, 365.0f, 40.0f, 25.0f, sf::Color(150,150,150), world),
     NonInteractable("Stone2", 380.0f, 365.0f, 40.0f, 25.0f, sf::Color(150,150,150), world),
     NonInteractable("Base2", 560.0f, 540.0f, 120.0f, 15.0f, sf::Color(139,90,43), world),
     NonInteractable("PostL3", 560.0f, 450.0f, 15.0f, 90.0f, sf::Color(139,90,43), world),
     NonInteractable("PostR3", 665.0f, 450.0f, 15.0f, 90.0f, sf::Color(139,90,43), world),
     NonInteractable("Shelf3", 560.0f, 450.0f, 120.0f, 15.0f, sf::Color(139,90,43), world),
     NonInteractable("Stone3", 600.0f, 425.0f, 40.0f, 25.0f, sf::Color(150,150,150), world)
 } };

    for (int i = 0; i < staticObjects.size(); i++) {
        staticObjects[i].getBody()->GetUserData().pointer = 200 + i;
    }

    //Setting up a wall for the ball to hit.
    b2BodyDef b2_wallDef;
    b2_wallDef.position.Set(750.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_wallBody = world.CreateBody(&b2_wallDef);


    b2PolygonShape b2_wallBox;
    b2_wallBox.SetAsBox(10.0f / SCALE, 80.0f / SCALE);
    b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);

    sf::RectangleShape sf_wallVisual(sf::Vector2f(20.0f, 160.0f));
    sf_wallVisual.setOrigin(10.0f, 80.0f);
    sf_wallVisual.setFillColor(sf::Color::Red);

    sf::CircleShape mouseCircle(20.0f);
    mouseCircle.setFillColor(sf::Color::Transparent);
    mouseCircle.setOutlineColor(sf::Color::Blue);
    mouseCircle.setOutlineThickness(2.0f);
    mouseCircle.setOrigin(20.0f, 20.0f);

    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/angry-birds.ttf"))
        std::cout << "Failed to load font" << std::endl;

    int pigCount = 3;

    sf::Text pigText;
    pigText.setFont(font);
    pigText.setString("Pigs: " + std::to_string(pigCount));
    pigText.setCharacterSize(24);
    pigText.setFillColor(sf::Color::White);
    pigText.setPosition(10.0f, 10.0f);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../assets/Ang_Birds/Backgrounds.png"))
        std::cout << "Failed to load background" << std::endl;

    sf::Texture skyTex, groundTex, grassTex, soilTex;
    skyTex.loadFromFile("../assets/Ang_Birds/Sky.png");
    groundTex.loadFromFile("../assets/Ang_Birds/Ground.png");
    grassTex.loadFromFile("../assets/Ang_Birds/Grass.png");
    soilTex.loadFromFile("../assets/Ang_Birds/Soil.png");

    sf::Sprite skySpr, groundSpr, grassSpr, treesSpr, soilSpr;

    skySpr.setTexture(skyTex);
    skySpr.setScale(800.0f / skyTex.getSize().x, 600.0f / skyTex.getSize().y);

    groundSpr.setTexture(groundTex);
    groundSpr.setScale(800.0f / groundTex.getSize().x, 50.0f / groundTex.getSize().y);
    groundSpr.setPosition(0.0f, 550.0f);

    grassSpr.setTexture(grassTex);
    grassSpr.setScale(800.0f / grassTex.getSize().x, 40.0f / grassTex.getSize().y);
    grassSpr.setPosition(0.0f, 510.0f);

    soilSpr.setTexture(soilTex);
    soilSpr.setScale(800.0f / soilTex.getSize().x, 30.0f / soilTex.getSize().y);
    soilSpr.setPosition(0.0f, 570.0f);

  

    bool showDecorations = false;

    // --- 7. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // INPUT HANDLING: Press SPACE to launch
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    pig1.applyImpulse(2.0f, -5.0f);
                }

                if (event.key.code == sf::Keyboard::D) {
                    showDecorations = !showDecorations;
                }

                if (event.key.code == sf::Keyboard::K) {
                    if (pigCount > 0) {
                        pigCount--;
                        pigText.setString("Pigs: " + std::to_string(pigCount));
                    }
                }

                if (event.key.code == sf::Keyboard::Space) {
                    if (!birdQueue.empty()) {
                        catapult.resetFired();
                        catapult.loadBird(birdQueue.front());
                        birdQueue.pop_front();
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    catapult.handleMousePress(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    if (catapult.isFired() && catapult.getLoadedBird())
                        catapult.getLoadedBird()->activate({ pig1.getBody(), pig2.getBody(), pig3.getBody() });
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                catapult.handleMouseMove(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    catapult.handleMouseRelease(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        auto hits = contactListener.getPointer();
        if (hits.count(3) && !pig1.isDestroyed()) pig1.takeDamage(100);
        if (hits.count(4) && !pig2.isDestroyed()) pig2.takeDamage(75);
        if (hits.count(5) && !pig3.isDestroyed()) pig3.takeDamage(100);

        if (hits.count(999) || pig1.getBody()->GetUserData().pointer == 999) pig1.takeDamage(1000);
        if (pig2.getBody()->GetUserData().pointer == 999) pig2.takeDamage(1000);
        if (pig3.getBody()->GetUserData().pointer == 999) pig3.takeDamage(1000);

        contactListener.s_ptr.clear();

        if (contactListener.s_blockHit) {
            for (int i = 0; i < (int)staticObjects.size(); i++) {
                if (!staticObjects[i].isDestroyed() &&
                    staticObjects[i].getBody()->GetUserData().pointer == contactListener.hitBlockPtr) {
                    staticObjects[i].onHit();
                }
            }
            contactListener.s_blockHit = false;
            contactListener.hitBlockPtr = 0;
        }

        if (!pig1.isDestroyed()) pig1.update();
        if (!pig2.isDestroyed()) pig2.update();
        if (!pig3.isDestroyed()) pig3.update();

        int livePigs = (!pig1.isDestroyed()) + (!pig2.isDestroyed()) + (!pig3.isDestroyed());
        pigText.setString("Pigs: " + std::to_string(livePigs));

        for (auto it = birdQueue.begin(); it != birdQueue.end(); ++it)
            (*it)->update();

        catapult.update();
        if (catapult.isFired() && catapult.getLoadedBird())
            catapult.getLoadedBird()->update();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            mouseCircle.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        }

        //All of the visuals needs to be synced with the physics.

        //Static objects usually don't move, but we set the position once.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue
        if (showDecorations) {
            window.draw(skySpr);
            window.draw(grassSpr);
            window.draw(groundSpr);
            window.draw(soilSpr);
        }

        window.draw(pigText);

        for (auto it = birdQueue.begin(); it != birdQueue.end(); ++it)
            (*it)->render(window);

        catapult.render(window);

        for (auto it = staticObjects.begin(); it != staticObjects.end(); ++it) {
            it->update();
            it->render(window);
        }

        if (!pig1.isDestroyed()) pig1.render(window);
        if (!pig2.isDestroyed()) pig2.render(window);
        if (!pig3.isDestroyed()) pig3.render(window);
        window.draw(mouseCircle); 

        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.display();
    }

    std::vector<DynamicObject*> mixedObjects;
    mixedObjects.push_back(&pig1);
    mixedObjects.push_back(&pig2);
    mixedObjects.push_back(&pig3);

    for (auto it = mixedObjects.begin(); it != mixedObjects.end(); ++it) {
        Bird* asBird = dynamic_cast<Bird*>(*it);
        if (asBird) { asBird->render(); continue; }

        Pig* asPig = dynamic_cast<Pig*>(*it);
        if (asPig) { asPig->render(window); }
    }

    for (auto obj : mixedObjects) {
        if (dynamic_cast<Bird*>(obj)) delete obj;
    }


    Bird redBird("Red", 1.0f, 10.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(0, 0, 80, 80), 100.0f, 200.0f);
    Pig smallPig(20.0f, 50, world, b2Vec2(300.0f / 30.0f, 400.0f / 30.0f), "../ assets / Ang_Birds / Pigs.png", sf::IntRect(0, 0, 120, 120));

    DynamicObject* obj1 = &redBird;
    DynamicObject* obj2 = &smallPig;
    DynamicObject* obj3 = &catapult;

    std::cout << "\n--- Upcasting Demo ---" << std::endl;
    obj1->update();
    obj2->update();
    obj3->update();

    std::cout << "\n--- Upcast to GameObject ---" << std::endl;
    GameObject* gameObj1 = &redBird;
    std::cout << "Type: " << gameObj1->getType() << std::endl;


    std::cout << "\n--- Smart Pointer Demo ---" << std::endl;
    std::unique_ptr<Bird> smartBird = std::make_unique<Bird>(
        "Red", 1.0f, 10.0f,
        "../assets/Ang_Birds/Angry_Birds.png",
        sf::IntRect(906, 797, 45, 51),
        100.0f, 200.0f
    );
    std::cout << "Smart Bird type: " << smartBird->getType() << std::endl;
    std::cout << "Smart pointer automatically cleaned up when out of scope" << std::endl;

    std::shared_ptr<Enemy> smartEnemy = std::make_shared<Enemy>(100);
    std::cout << "Smart Enemy health: " << smartEnemy->getHealth() << std::endl;
    std::cout << "Shared pointer reference count: " << smartEnemy.use_count() << std::endl;

    return 0;
}