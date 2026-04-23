#pragma once
#include "StaticObject.h"
#include "DynamicObject.h"

class UI : public virtual StaticObject, public virtual DynamicObject {
private:
    std::string str_label;

public:
    UI(std::string label, float posX = 0.0f, float posY = 0.0f)
        : StaticObject(posX, posY), DynamicObject(posX, posY), str_label(label) {
    }

    void update() override {
        std::cout << "UI update: " << str_label << std::endl;
    }

    void render() override {
        std::cout << "UI render: " << str_label << std::endl;
    }

    std::string getType() const override { return "UI"; }

    virtual ~UI() {
        std::cout << "UI destroyed" << std::endl;
    }
};