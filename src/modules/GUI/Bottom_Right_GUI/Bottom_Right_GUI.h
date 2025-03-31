#pragma once
#include "ofxDatGui.h"
#include "modules/Configuration/Configuration.h"
#include <string>
#include <json.hpp>

class GUI;

class Bottom_Right_GUI {
public:
    Bottom_Right_GUI();
    void setup(GUI* gui_manager);
    ofxDatGui* getGui();

    bool isMouseOverGui(int x, int y) const;
private:
    GUI* gui_manager;
    ofxDatGui* gui;

    void setupCategories(const nlohmann::json& config);
    void createComponent(ofxDatGuiFolder* folder, const std::string& type, const std::string& label, const nlohmann::json& details);

    float safeStof(const std::string& str);
    int stringToHex(const std::string& hexStr);
};
