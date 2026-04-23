#pragma once
#include <iostream>
#include <string>

class GameObject {
public:
    virtual void update() = 0;
    virtual void render() = 0;
    virtual std::string getType() const = 0;
    virtual ~GameObject() {
        std::cout << "GameObject destroyed" << std::endl;
    }
};