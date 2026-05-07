#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h"
#include "Pig.h"
#include "Catapult.h"
#include "DynamicObject.h"
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

    //Can set a definition for PI.
    const float PI = 3.1415927;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    Pig pig1(20.0f, 100, world, b2Vec2(200.0f / 30.0f, 300.0f / 30.0f), window, "../assets/Ang_Birds/sprite_1.png", sf::IntRect(4, 5, 56, 47));
    Pig pig2(30.0f, 150, world, b2Vec2(400.0f / 30.0f, 200.0f / 30.0f), window, "../assets/Ang_Birds/sprite_2.png", sf::IntRect(5, 0, 89, 100));
    Pig pig3(40.0f, 200, world, b2Vec2(600.0f / 30.0f, 100.0f / 30.0f), window, "../assets/Ang_Birds/sprite_4.png", sf::IntRect(2, 8, 103, 98));

    std::list<Bird*> birdQueue;

    std::vector<std::tuple<std::string, float, float, std::string, sf::IntRect, float, float>> birdData = {
        {"Red",   1.0f, 10.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(906,797,45,51),   100.0f, 560.0f},
        {"Chuck", 0.8f, 15.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(667,879,61,63),  160.0f, 560.0f},
        {"Bomb",  2.0f,  8.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(408,726,65,118), 220.0f, 560.0f},
        {"Matilda", 0.5f, 18.0f, "../assets/Ang_Birds/Angry_Birds.png", sf::IntRect(418,638,73,128), 300.0f, 560.0f}
    };

    for (auto& [type, mass, speed, path, rect, x, y] : birdData) {
        birdQueue.push_back(new Bird(type, mass, speed, path, rect, x, y));
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

    sf::CircleShape mouseCircle(20.0f);
    mouseCircle.setFillColor(sf::Color::Blue);
    mouseCircle.setOrigin(20.0f, 20.0f);

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

    struct StaticObject {
        sf::RectangleShape shape;
        StaticObject(float x, float y, float w, float h, sf::Color c) {
            shape.setSize(sf::Vector2f(w, h));
            shape.setPosition(x, y);
            shape.setFillColor(c);
        }
    };

    std::array<StaticObject, 2> staticObjects = { {
        StaticObject(450.0f, 510.0f, 20.0f, 80.0f,  sf::Color(180,180,180)),
        StaticObject(580.0f, 530.0f, 60.0f, 60.0f,  sf::Color(173,216,230))
    } };

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

    //Rather than having an immovable wall, we can use the dynamic body type to create one that can have velocity etc.
    b2BodyDef b2_plankDef;

    b2_plankDef.type = b2_dynamicBody;
    b2_plankDef.position.Set(550.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_plankBody = world.CreateBody(&b2_plankDef);

    b2PolygonShape b2_plankBox;
    b2_plankBox.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

    b2FixtureDef b2_plankFixture;
    b2_plankFixture.shape = &b2_plankBox;
    b2_plankFixture.density = 1.5f;   // Light wood
    b2_plankFixture.friction = 0.3f;
    b2_plankBody->CreateFixture(&b2_plankFixture);

    sf::RectangleShape sf_plankVisual(sf::Vector2f(20.0f, 120.0f));
    sf_plankVisual.setOrigin(10.0f, 60.0f);
    sf_plankVisual.setFillColor(sf::Color(139, 69, 19)); // Brown

    //Create a ball that is fired when space is pressed. We need to first have a dynamic ball to do it.
    b2BodyDef b2_ballDef;
    b2_ballDef.type = b2_dynamicBody;
    b2_ballDef.position.Set(100.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_ballBody = world.CreateBody(&b2_ballDef);

    b2CircleShape b2_circleShape;
    b2_circleShape.m_radius = 15.0f / SCALE;

    b2FixtureDef b2_ballFixture;
    b2_ballFixture.shape = &b2_circleShape;
    b2_ballFixture.density = 1.0f;
    b2_ballFixture.restitution = 0.5f; // Bounciness
    b2_ballBody->CreateFixture(&b2_ballFixture);

    sf::CircleShape sf_ballVisual(15.0f);
    sf_ballVisual.setOrigin(15.0f, 15.0f);
    sf_ballVisual.setFillColor(sf::Color::Yellow);

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

    sf::Texture skyTex, groundTex, grassTex, treesTex, soilTex;
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
                        b2_ballBody->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
                        b2_ballBody->SetLinearVelocity(b2Vec2(0, 0));
                        b2_ballBody->SetAngularVelocity(0);
                        b2_ballBody->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_ballBody->GetWorldCenter(), true);

                        Bird* fired = birdQueue.front();
                        birdQueue.pop_front();
                        delete fired;
                    }
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        pig1.update();
        pig2.update();
        pig3.update();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            mouseCircle.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        }

        //All of the visuals needs to be synced with the physics.

        sf_ballVisual.setPosition(b2_ballBody->GetPosition().x * SCALE, b2_ballBody->GetPosition().y * SCALE);
        sf_ballVisual.setRotation(b2_ballBody->GetAngle() * (180.0f / PI));

        //Static objects usually don't move, but we set the position once.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        // Dynamic wall.
        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue
        if (showDecorations) {
            window.draw(skySpr);
            window.draw(treesSpr);
            window.draw(grassSpr);
            window.draw(groundSpr);
            window.draw(soilSpr);
        }

        window.draw(pigText);

        for (auto it = birdQueue.begin(); it != birdQueue.end(); ++it)
            (*it)->render(window);

        for (auto it = staticObjects.begin(); it != staticObjects.end(); ++it)
            window.draw(it->shape);

        pig1.render(window);
        pig2.render(window);
        pig3.render(window);
        window.draw(mouseCircle); 

        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sf_ballVisual);

        window.display();
    }

    std::vector<DynamicObject*> mixedObjects;
    mixedObjects.push_back(&pig1);
    mixedObjects.push_back(&pig2);
    mixedObjects.push_back(&pig3);

    for (auto& [type, mass, speed, path, rect, x, y] : birdData) {
        mixedObjects.push_back(new Bird(type, mass, speed, path, rect, x, y));
    }

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
    Pig smallPig(20.0f, 50, world, b2Vec2(300.0f / 30.0f, 400.0f / 30.0f), window, "../ assets / Ang_Birds / Pigs.png", sf::IntRect(0, 0, 120, 120));
    Catapult catapult(50.0f, 500.0f);

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

    return 0;
}