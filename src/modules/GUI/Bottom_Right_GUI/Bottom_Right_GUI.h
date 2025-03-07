#pragma once
#include "ofxDatGui.h"
#include <vector>
#include <string>

#include "modules/Configuration/Configuration.h"

class GUI;

class Bottom_Right_GUI {
public:
    Bottom_Right_GUI();
    void setup(GUI* gui_manager);

private:
    GUI* gui_manager;
    ofxDatGui* gui;
};
