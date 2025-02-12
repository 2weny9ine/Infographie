#pragma once
#include "ofxDatGui.h"

class Top_Right_GUI
{
public:
	ofxDatGui* gui;

	Top_Right_GUI();

private:
	ofxDatGuiFolder* position_folder;
	ofxDatGuiTextInput* x_position;
	ofxDatGuiTextInput* y_position;
	ofxDatGuiTextInput* z_position;
};