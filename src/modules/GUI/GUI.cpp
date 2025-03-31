#include "GUI.h"
#include "Top_Left_GUI/Top_Left_GUI.h"
#include "Top_Right_GUI/Top_Right_GUI.h"
#include "Bottom_Left_GUI/Bottom_Left_GUI.h"
#include "Bottom_Right_GUI/Bottom_Right_GUI.h"

GUI::GUI()
    : scene(nullptr),
    top_left(nullptr),
    top_right(nullptr),
    bottom_left(nullptr),
    bottom_right(nullptr)
{}

GUI::~GUI() {

}

void GUI::setup(Scene* scene) {
    this->scene = scene;
    initializeGUIs();
}

void GUI::initializeGUIs() {
    top_left = std::make_unique<Top_Left_GUI>();
    top_right = std::make_unique<Top_Right_GUI>();
    bottom_left = std::make_unique<Bottom_Left_GUI>();
    bottom_right = std::make_unique<Bottom_Right_GUI>();
    guis.push_back(top_left.get()->getGui());
    guis.push_back(top_right.get()->getGui());
    guis.push_back(bottom_left.get()->getGui());
    guis.push_back(bottom_right.get()->getGui());
    top_left->setup(this);
    top_right->setup(this);
    bottom_left->setup(this);
    bottom_right->setup(this);
}

Scene* GUI::getScene() {
    return scene;
}

bool GUI::isMouseOverGui(int x, int y) const
{
    return top_left->isMouseOverGui(x, y) ||
        top_right->isMouseOverGui(x, y) ||
        bottom_left->isMouseOverGui(x, y) ||
        bottom_right->isMouseOverGui(x, y);
}
