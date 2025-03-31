#pragma once
#include "ofxDatGui.h"
#include <vector>
#include <string>

#include "objects/Object3D.h"
#include "objects/3D/Primitives/Cube/Cube.h"
#include "objects/3D/Primitives/Sphere/Sphere.h"
#include "objects/3D/Primitives/Cylinder/Cylinder.h"
#include "objects/3D/Primitives/Cone/Cone.h"
#include "modules/Properties/ModularProperties.h"

class GUI;

class Bottom_Left_GUI {
public:
    Bottom_Left_GUI();
    void setup(GUI* gui_manager);

    void resetTransformations();
    void updatePropertyControls(); // Dynamically create controls based on selected object's properties

    std::vector<float> localTransformations;

    ofxDatGui* getGui();
    bool isMouseOverGui(int x, int y) const;

private:
    GUI* gui_manager;
    ofxDatGui* gui;

    // Shape creation UI
    std::vector<std::string> shape_options;
    ofxDatGuiDropdown* shape_dropdown;
    ofxDatGuiColorPicker* shape_color_picker;
    ofxDatGuiSlider* shape_scale_slider;
    ofxDatGuiSlider* shape_opacity_slider;
    ofxDatGuiButton* create_shape_btn;

    // Folder for dynamic attributes
    ofxDatGuiFolder* attributes_folder;

    // Transform & duplication buttons
    ofxDatGuiButton* translateButton;
    ofxDatGuiButton* rotateButton;
    ofxDatGuiButton* scaleButton;
    ofxDatGuiButton* duplicateInstanceButton;

    void createShape(ofxDatGuiButtonEvent e);
};
