#include "Bottom_Right_GUI.h"
#include "modules/GUI/GUI.h"
#include <iostream>

Bottom_Right_GUI::Bottom_Right_GUI()
	: gui_manager(nullptr), gui(nullptr)
{

	gui = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_RIGHT);
	gui->addHeader(":: Configuration Menu ::");
	gui->addFooter();
	gui->getFooter()->setLabelWhenExpanded("CLOSE CONFIGURATION MENU");
	gui->getFooter()->setLabelWhenCollapsed("EXPAND CONFIGURATION MENU");
	gui->getHeader()->setDraggable(true);
	gui->collapse();

}

void Bottom_Right_GUI::setup(GUI* gui_manager)
{
	this->gui_manager = gui_manager;
}

