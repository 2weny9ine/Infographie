#pragma once

#include "ofxDatGui.h"
#include "Scene.h"
#include "Top_Left_GUI/Top_Left_GUI.h"
#include "Top_Right_GUI/Top_Right_GUI.h"
#include "Bottom_Left_GUI/Bottom_Left_GUI.h"

class GUI {
public:
    GUI();
    ~GUI();
    void setup(Scene* scene);
    Scene* getScene();

    std::unique_ptr<Top_Left_GUI> top_left;
    std::unique_ptr<Top_Right_GUI> top_right;
    std::unique_ptr<Bottom_Left_GUI> bottom_left;
private:
    Scene* scene;

    void initializeGUIs();
};
