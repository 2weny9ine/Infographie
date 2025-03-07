#pragma once
#include "ofxDatGui.h"
#include "modules/Configuration/Configuration.h"

class GUI;

class Bottom_Right_GUI {
public:
    Bottom_Right_GUI();
    void setup(GUI* gui_manager);

private:
    GUI* gui_manager;
    ofxDatGui* gui;

    void setupCategories(const nlohmann::json& config);
    void createComponent(ofxDatGuiFolder* folder, const std::string& type, const std::string& label, const nlohmann::json& details);
};
