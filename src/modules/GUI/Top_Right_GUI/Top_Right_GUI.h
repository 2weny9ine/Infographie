#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "objects/Object3D.h"

//2.3
enum class PrimitiveType { POINT, LINE, RECTANGLE, CIRCLE, ELLIPSE, TRIANGLE};

class GUI;
class Scene;
class Top_Right_GUI
{
public:
    ofxDatGui* gui;
    Top_Right_GUI();
    
    void setup(GUI* gui_manager);
    void deleteSelected();
    
    ofColor getBackgroundColor() const { return backgroundColorPicker->getColor(); }
    ofColor getStrokeColor() const { return strokeColorPicker->getColor(); }
    ofColor getFillColor() const { return fillColorPicker->getColor(); }
    float getLineWidth() const { return lineWidthSlider->getValue(); }
    //2.3
    PrimitiveType getSelectedPrimitive() const { return selectedPrimitive; }
    bool isDrawingEnabled() const { return drawingToggle->getChecked(); }
    bool isOutlineEnabled() const { return outlineToggle->getChecked(); }
    int  getTopologyPoints() const;
    
    void addObjectToggle(Object3D* object);
    std::vector<ofxDatGuiButton*> objectButtons;
    
    ofxDatGui* getGui();
    
    bool isMouseOverGui(int x, int y) const;
    
private:
    ofxDatGuiFolder* objectsFolder;
    ofxDatGuiButton* deleteButton;
    ofxDatGuiFolder* toolsFolder;
    ofxDatGuiColorPicker* backgroundColorPicker;
    ofxDatGuiColorPicker* strokeColorPicker;
    ofxDatGuiColorPicker* fillColorPicker;
    ofxDatGuiSlider* lineWidthSlider;
    
    bool outlineEnabled = false;
    int  counter;
    
    //2.3
    ofxDatGuiToggle* drawingToggle;
    ofxDatGuiTextInput* topologyInput; 
    ofxDatGuiToggle* pointToggle;
    ofxDatGuiToggle* lineToggle;
    ofxDatGuiToggle* rectangleToggle;
    ofxDatGuiToggle* circleToggle;
    ofxDatGuiToggle* ellipseToggle;
    ofxDatGuiToggle* triangleToggle;
    ofxDatGuiToggle* outlineToggle;
    PrimitiveType selectedPrimitive = PrimitiveType::RECTANGLE;
    void onPrimitiveSelected(ofxDatGuiToggleEvent e);
    void onOutlineToggle(ofxDatGuiToggleEvent e);
    
    GUI* gui_manager;
    
    ofxDatGuiFolder* topologyFolder;
    ofxDatGuiToggle* topologyToggle;
    ofxDatGuiSlider* topologySlider;    
    
    void onTopologyToggle(ofxDatGuiToggleEvent e);
    void onTopologySlider(ofxDatGuiSliderEvent e);
};
