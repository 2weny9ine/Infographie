#pragma once
#include "ofxDatGui.h"
#include <vector>
#include <string>

#include "objects/Object3D.h"
#include "objects/ModelObject/Templates/Cube/Cube.h"
#include "objects/ModelObject/Templates/Sphere/Sphere.h"
#include "objects/ModelObject/Templates/Cylinder/Cylinder.h"
#include "objects/ModelObject/Templates/Cone/Cone.h"

class GUI;

class Bottom_Left_GUI {
public:
    Bottom_Left_GUI();
    void setup(GUI* gui_manager);

private:
    GUI* gui_manager;
    ofxDatGui* gui;

    ofxDatGuiLabel* create_category_label;
    std::vector<std::string> shape_options;
    ofxDatGuiDropdown* shape_dropdown;
    ofxDatGuiColorPicker* shape_color_picker;
    ofxDatGuiSlider* shape_scale_slider;
    ofxDatGuiSlider* shape_opacity_slider;
    ofxDatGuiButton* create_shape_btn;

    void createShape(ofxDatGuiButtonEvent e);
};
