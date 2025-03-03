#include "Bottom_Left_GUI.h"
#include "modules/GUI/GUI.h"
#include <iostream>

Bottom_Left_GUI::Bottom_Left_GUI()
	: gui_manager(nullptr), gui(nullptr),
	create_category_label(nullptr),
	shape_dropdown(nullptr), create_shape_btn(nullptr) {

	gui = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_LEFT);
	gui->addHeader(":: Create Menu ::");
	gui->addFooter();
	gui->getFooter()->setLabelWhenExpanded("CLOSE CREATE MENU");
	gui->getFooter()->setLabelWhenCollapsed("EXPAND CREATE MENU");
	gui->getHeader()->setDraggable(false);

	shape_options = { "CUBE", "SPHERE" };

	shape_dropdown = gui->addDropdown("Shape", shape_options);
	shape_dropdown->setStripeColor(ofColor::red);

	create_shape_btn = gui->addButton("Add to workspace");
	create_shape_btn->setStripeColor(ofColor::darkRed);
	create_shape_btn->onButtonEvent(this, &Bottom_Left_GUI::createShape);
}

void Bottom_Left_GUI::setup(GUI* gui_manager) {
	this->gui_manager = gui_manager;
}

void Bottom_Left_GUI::createShape(ofxDatGuiButtonEvent e) {
	std::string selectedShape = shape_dropdown->getSelected()->getName();

	Object3D* modelObj = nullptr;

	if (selectedShape == "CUBE") {
		modelObj = new Cube();
	}
	else if (selectedShape == "SPHERE") {
		modelObj = new Sphere();
	}

	if (modelObj) {
		gui_manager->getScene()->addObject(modelObj);
	}
}
