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

	shape_color_picker->onColorPickerEvent([this](ofxDatGuiColorPickerEvent e)
	{
		for (Object3D* object : gui_manager->getScene()->selectedObjects)
		{
			object->setColor(shape_color_picker->getColor());
		}
	});

	shape_scale_slider->onSliderEvent([this](ofxDatGuiSliderEvent e)
	{
		for (Object3D* object : gui_manager->getScene()->selectedObjects)
		{
			object->setScale(ofVec3f(shape_scale_slider->getValue(), shape_scale_slider->getValue(), shape_scale_slider->getValue()));
		}
	});

	shape_opacity_slider->onSliderEvent([this](ofxDatGuiSliderEvent e)
	{
		for (Object3D* object : gui_manager->getScene()->selectedObjects)
		{
			object->setOpacity(shape_opacity_slider->getValue());
		}
	});

	shape_color_picker->onSliderEvent([this](ofxDatGuiSliderEvent e)
	{
		for (Object3D* object : gui_manager->getScene()->selectedObjects)
		{
			object->setScale(ofVec3f(shape_scale_slider->getValue(), shape_scale_slider->getValue(), shape_scale_slider->getValue()));
		}
	});




	attributes_folder = gui->addFolder("Attributes", ofColor::white);

	inputs = {

	x_position = attributes_folder->addTextInput("Position X", "N/A"),

	y_position = attributes_folder->addTextInput("Position Y", "N/A"),

	z_position = attributes_folder->addTextInput("Position Z", "N/A"),

	x_rotation = attributes_folder->addTextInput("Rotation X", "N/A"),

	y_rotation = attributes_folder->addTextInput("Rotation Y", "N/A"),

	z_rotation = attributes_folder->addTextInput("Rotation Z", "N/A"),

	x_scale = attributes_folder->addTextInput("Scale X", "N/A"),

	y_scale = attributes_folder->addTextInput("Scale Y", "N/A"),

	z_scale = attributes_folder->addTextInput("Scale Z", "N/A")

	};

	translateButton = attributes_folder->addButton("Translate [T]");
	rotateButton = attributes_folder->addButton("Rotate [R]");
	scaleButton = attributes_folder->addButton("Resize [P]");
	duplicateInstanceButton = attributes_folder->addButton("Duplicate Instance [CTRL + D]");

	for (ofxDatGuiTextInput* input : inputs)
	{
		input->setInputType(ofxDatGuiInputType::NUMERIC);
		input->onTextInputEvent([this](ofxDatGuiTextInputEvent e)
		{
			SendTransformations();
		});
	}

	translateButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
	{
		if (gui_manager->getScene()->currentTransform != Scene::TransformMode::Translate)
			gui_manager->getScene()->currentTransform = Scene::TransformMode::Translate;
		else
			gui_manager->getScene()->currentTransform = Scene::TransformMode::None;
	});
	rotateButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
	{
		if (gui_manager->getScene()->currentTransform != Scene::TransformMode::Rotate)
			gui_manager->getScene()->currentTransform = Scene::TransformMode::Rotate;
		else
			gui_manager->getScene()->currentTransform = Scene::TransformMode::None;
	});
	scaleButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
	{
		if (gui_manager->getScene()->currentTransform != Scene::TransformMode::Resize)
			gui_manager->getScene()->currentTransform = Scene::TransformMode::Resize;
		else
			gui_manager->getScene()->currentTransform = Scene::TransformMode::None;
	});

	duplicateInstanceButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
	{
		gui_manager->getScene()->duplicateSelectedInstances();
	});
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

void Bottom_Left_GUI::resetTransformations()
{
	for (ofxDatGuiTextInput* input : inputs)
	{
		input->setText("--");
	}
}

ofxDatGui* Bottom_Left_GUI::getGui()
{
	return gui;
}

void Bottom_Left_GUI::SendTransformations()
{
	float tx, ty, tz, rx, ry, rz, sx, sy, sz;
	std::vector<float> transformations;
	transformations = { tx = 0, ty = 0, tz = 0, rx = 0, ry = 0, rz = 0, sx = 0, sy = 0,sz = 0 };

	for (size_t i = 0; i < transformations.size(); i++)
	{
		if (inputs[i]->getText() == "--")
			transformations[i] = 0;
		else
		{
			transformations[i] = std::stof(inputs[i]->getText());
		}
	}

	ofVec3f position = ofVec3f{ transformations[0] - localTransformations[0], transformations[1] - localTransformations[1],transformations[2] - localTransformations[2] };
	ofVec3f rotation = ofVec3f{ transformations[3] - localTransformations[3], transformations[4] - localTransformations[4],transformations[5] - localTransformations[5] };
	ofVec3f scale = ofVec3f{ transformations[6] - localTransformations[6], transformations[7] - localTransformations[7],transformations[8] - localTransformations[8] };
	gui_manager->getScene()->apply_Transformations(position, rotation, scale);

	if (gui_manager->getScene()->selectedObjects.size() != 1)
		localTransformations = transformations;
}