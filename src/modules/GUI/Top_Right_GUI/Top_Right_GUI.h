#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"


//2.3
enum class PrimitiveType { POINT, LINE, RECTANGLE, CIRCLE, ELLIPSE };

class Top_Right_GUI
{
public:
    ofxDatGui* gui;
    Top_Right_GUI();

    ofColor getBackgroundColor() const { return backgroundColorPicker->getColor(); }
    ofColor getStrokeColor() const { return strokeColorPicker->getColor(); }
    ofColor getFillColor() const { return fillColorPicker->getColor(); }
    float getLineWidth() const { return lineWidthSlider->getValue(); }
    //2.3
    PrimitiveType getSelectedPrimitive() const { return selectedPrimitive; }

private:
    ofxDatGuiFolder* toolsFolder;
    ofxDatGuiColorPicker* backgroundColorPicker;
    ofxDatGuiColorPicker* strokeColorPicker;
    ofxDatGuiColorPicker* fillColorPicker;
    ofxDatGuiSlider* lineWidthSlider;

    //2.3
    ofxDatGuiToggle* pointToggle;
    ofxDatGuiToggle* lineToggle;
    ofxDatGuiToggle* rectangleToggle;
    ofxDatGuiToggle* circleToggle;
    ofxDatGuiToggle* ellipseToggle;
    PrimitiveType selectedPrimitive = PrimitiveType::RECTANGLE;
    void onPrimitiveSelected(ofxDatGuiToggleEvent e);
};