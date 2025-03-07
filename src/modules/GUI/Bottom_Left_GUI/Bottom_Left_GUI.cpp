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

	shape_options = { "CUBE", "SPHERE", "CYLINDER", "CONE" };

	shape_dropdown = gui->addDropdown("Shape", shape_options);
	shape_dropdown->setStripeColor(ofColor::darkRed);
	for (int i = 0; i < shape_dropdown->size(); i++)
	{
		shape_dropdown->getChildAt(i)->setStripeColor(ofColor::red);
	}
	
	shape_color_picker = gui->addColorPicker("COLOR", ofColor::grey);
	shape_color_picker->setStripeColor(ofColor::darkRed);

	shape_scale_slider = gui->addSlider("SCALE", 1.0f, 100.0f);
	shape_scale_slider->setValue(50.0f),
	shape_scale_slider->setPrecision(0);
	shape_scale_slider->setStripeColor(ofColor::darkRed);

	shape_opacity_slider = gui->addSlider("OPACITY", 0.0f, 1.0f);
	shape_opacity_slider->setValue(1.0f),
	shape_opacity_slider->setStripeColor(ofColor::darkRed);

	create_shape_btn = gui->addButton("Add to workspace");
	create_shape_btn->setStripeVisible(false);
	create_shape_btn->setBackgroundColor(ofColor::darkRed);
	create_shape_btn->setLabelAlignment(ofxDatGuiAlignment::CENTER);
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
	else if (selectedShape == "SPHERE")
	{
		modelObj = new Sphere();
	}
	else if (selectedShape == "CYLINDER")
	{
		modelObj = new Cylinder();
	}
	else if (selectedShape == "CONE")
	{
		modelObj = new Cone();
	}

	if (modelObj) {
		ofColor model_color = shape_color_picker->getColor();
		modelObj->setColor(model_color);

		float model_size = shape_scale_slider->getValue();
		ofVec3f model_scale(model_size, model_size, model_size);
		modelObj->setScale(model_scale);

		float model_opacity = shape_opacity_slider->getValue();
		modelObj->setOpacity(model_opacity);

		gui_manager->getScene()->addObject(modelObj);
	}
}
