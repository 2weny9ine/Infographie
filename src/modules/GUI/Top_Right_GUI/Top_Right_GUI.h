#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"
#include "objects/Object3D.h"

//2.3
enum class PrimitiveType { POINT, LINE, RECTANGLE, CIRCLE, ELLIPSE, TRIANGLE};

class GUI;

class Top_Right_GUI
{
public:
    ofxDatGui* gui;
    Top_Right_GUI();

	void setup(GUI* gui_manager);

    ofColor getBackgroundColor() const { return backgroundColorPicker->getColor(); }
    ofColor getStrokeColor() const { return strokeColorPicker->getColor(); }
    ofColor getFillColor() const { return fillColorPicker->getColor(); }
    float getLineWidth() const { return lineWidthSlider->getValue(); }
    //2.3
    PrimitiveType getSelectedPrimitive() const { return selectedPrimitive; }
    bool isDrawingEnabled() const { return drawingToggle->getChecked(); } // New function to check if drawing mode is enabled
    bool isOutlineEnabled() const { return outlineToggle->getChecked(); }

    void addObjectToggle(Object3D* object);

    ofxDatGui* getGui();
private:
    ofxDatGuiButton* lastObjectButton;

    ofxDatGuiFolder* objectsFolder;
    ofxDatGuiButton* deleteButton;
    ofxDatGuiFolder* toolsFolder;
    ofxDatGuiColorPicker* backgroundColorPicker;
    ofxDatGuiColorPicker* strokeColorPicker;
    ofxDatGuiColorPicker* fillColorPicker;
    ofxDatGuiSlider* lineWidthSlider;

    bool outlineEnabled = false;
    int counter;

    //2.3
    ofxDatGuiToggle* drawingToggle;

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

};