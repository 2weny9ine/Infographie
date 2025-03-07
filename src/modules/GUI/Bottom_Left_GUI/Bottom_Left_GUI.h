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

    void resetTransformations();

    std::vector<ofxDatGuiTextInput*> inputs;
    std::vector<float>localTransformations = { 0,0,0,0,0,0,0,0,0 };
    ofxDatGui* getGui();
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

    ofxDatGuiFolder* attributes_folder;
    ofxDatGuiTextInput* x_position;
    ofxDatGuiTextInput* y_position;
    ofxDatGuiTextInput* z_position;
    ofxDatGuiTextInput* x_rotation;
    ofxDatGuiTextInput* y_rotation;
    ofxDatGuiTextInput* z_rotation;
    ofxDatGuiTextInput* x_scale;
    ofxDatGuiTextInput* y_scale;
    ofxDatGuiTextInput* z_scale;
    ofxDatGuiButton* translateButton;
    ofxDatGuiButton* rotateButton;
    ofxDatGuiButton* scaleButton;
    ofxDatGuiButton* duplicateInstanceButton;

    void SendTransformations();

    void createShape(ofxDatGuiButtonEvent e);
};
