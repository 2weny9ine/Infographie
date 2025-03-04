#pragma once
#include "ofxDatGui.h"
#include "Image.h"

class GUI;

class Top_Left_GUI
{
public:
    Top_Left_GUI();
    void update();

    void setImage(Image* img) { imagePtr = img; }

    void setup(GUI* gui_manager);
    bool colorFilterEnabled() const { return applyColors; }
    bool histogramEnabled() const;
    
    int getX() const { return gui->getPosition().x; }
    int getY() const { return gui->getPosition().y; }
    int getHeight() const { return gui->getHeight(); }



    

    ofColor getRGBColor();
    ofColor getHSBColor();
    
private:
    GUI* gui_manager;
    ofxDatGui* gui;
    
    ofxDatGuiFolder* position_folder;
    ofxDatGuiTextInput* x_position;
    ofxDatGuiTextInput* y_position;
    ofxDatGuiTextInput* z_position;
    
    ofxDatGuiFolder* create_folder;
    vector<string> shape_options;
    ofxDatGuiDropdown* shape_dropdown;
    ofxDatGuiSlider* exportDurationSlider; 
    ofxDatGuiButton* exportButton;
    
    ofxDatGuiFolder* histogram_folder;
    ofxDatGuiButton* histogramButton;
    ofxDatGuiButton* deleteImagesButton;
    bool showHistogram;
    
    ofxDatGuiFolder* color_folder;
    ofxDatGuiColorPicker* colorPickerRGB;
    ofxDatGuiSlider* hueValue;
    ofxDatGuiSlider* satValue;
    ofxDatGuiSlider* briValue;
    ofxDatGuiButton* toggleColorButton;
    bool applyColors;
    Image* imagePtr;
    
    
    

};
