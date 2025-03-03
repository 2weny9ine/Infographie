#pragma once
#include "ofxDatGui.h"

class GUI;

class Top_Left_GUI
{
public:
	Top_Left_GUI();

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
};