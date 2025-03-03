#pragma once

#include "ofxDatGui.h"
#include "Scene.h"

class Top_Left_GUI;
class Top_Right_GUI;
class Bottom_Left_GUI;

class GUI {
public:
    GUI();
    ~GUI();
    void setup(Scene* scene);
    Scene* getScene();

private:
    Scene* scene;

    std::unique_ptr<Top_Left_GUI> top_left;
    std::unique_ptr<Top_Right_GUI> top_right;
    std::unique_ptr<Bottom_Left_GUI> bottom_left;

    void initializeGUIs();
};
