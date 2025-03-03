#pragma once
#include "ofxDatGui.h"
#include "Image.h"

class Top_Left_GUI
{
public:
    ofxDatGui* gui;
    
    Top_Left_GUI();
    void update();
    void draw();
    void setImage(Image* img) { imagePtr = img; }
    
private:
    ofxDatGuiFolder* position_folder;
    ofxDatGuiTextInput* x_position;
    ofxDatGuiTextInput* y_position;
    ofxDatGuiTextInput* z_position;
    
    
    ofxDatGuiFolder* histogram_folder;
    ofxDatGuiButton* histogramButton;
    ofxDatGuiButton* deleteImagesButton;
    bool showHistogram;
    

    Image* imagePtr;
};
