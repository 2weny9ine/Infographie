#pragma once

#include "ofxDatGui.h"
#include "Top_Left_GUI/Top_Left_GUI.h"
#include "Top_Right_GUI/Top_Right_GUI.h"

class GUI
{
public:
	GUI();

	Top_Left_GUI top_left;
	Top_Right_GUI top_right;
};