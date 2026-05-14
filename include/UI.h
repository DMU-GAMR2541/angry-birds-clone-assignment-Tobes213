#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"

// UI class using multiple inheritance
class UI : public virtual StaticObject, public virtual DynamicObject {
private:
     
    //  Stores UI text label
    std::string str_label;

public:
    UI(std::string label, float posX = 0.0f, float posY = 0.0f)
        : StaticObject(posX, posY), DynamicObject(posX, posY), str_label(label) {
    }

    // Updates UI elements
    void update() override {
        std::cout << "UI update: " << str_label << std::endl;
    }

    // Renders UI elements
    void render() override {
        std::cout << "UI render: " << str_label << std::endl;
    }

    std::string getType() const override { return "UI"; }

    // Destructor message for debugging
    virtual ~UI() {
        std::cout << "UI destroyed" << std::endl;
    }
};