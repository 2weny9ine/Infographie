#pragma once
#include "ofxDatGui.h"

class GUI;

class Top_Right_GUI
{
public:
	Top_Right_GUI();

	void setup(GUI* gui_manager);

private:
	
	GUI* gui_manager;
	ofxDatGui* gui;

};