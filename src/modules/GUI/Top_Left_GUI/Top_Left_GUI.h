#pragma once
#include "ofxDatGui.h"
#include "Image.h"

class GUI;

class Top_Left_GUI
{
public:
	Top_Left_GUI();
    void update();
    void draw();
    void setImage(Image* img) { imagePtr = img; }
    
	void setup(GUI* gui_manager);

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

    ofxDatGuiFolder* histogram_folder;
    ofxDatGuiButton* histogramButton;
    ofxDatGuiButton* deleteImagesButton;
    bool showHistogram;

    Image* imagePtr;
};