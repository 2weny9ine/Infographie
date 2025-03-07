#include "Top_Right_GUI.h"
#include "modules/GUI/GUI.h"


Top_Right_GUI::Top_Right_GUI()
{
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    gui->setWidth(200);

    objectsFolder = gui->addFolder("Objects", ofColor::white);
    objectsFolder->expand();
    deleteButton = objectsFolder->addButton("Delete Object");
    deleteButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        for (Object3D* object : gui_manager->getScene()->selectedObjects)
        {
            gui_manager->getScene()->removeObject(object);
        }
        gui_manager->getScene()->resetSelection();
        gui_manager->getScene()->update_Attributes();

        auto it = std::find(objectsFolder->children.begin(), objectsFolder->children.end(), lastObjectButton);
        if (it != objectsFolder->children.end())
        {
            objectsFolder->children.erase(it);
        }
        objectsFolder->expand();
        });

    toolsFolder = gui->addFolder("Drawing Tools", ofColor::white);
    //toolsFolder->expand();

    backgroundColorPicker = toolsFolder->addColorPicker("Background", ofColor(31));
    strokeColorPicker = toolsFolder->addColorPicker("Outline", ofColor(255));
    fillColorPicker = toolsFolder->addColorPicker("Fill", ofColor(127, 127, 127, 127));
    lineWidthSlider = toolsFolder->addSlider("Line Width", 0.5f, 10.0f, 2.0f);

    drawingToggle = gui->addToggle("Enable Drawing", false);

    //2.3
    pointToggle = toolsFolder->addToggle("Point");
    outlineToggle = toolsFolder->addToggle("No Fill", false);
    lineToggle = toolsFolder->addToggle("Line");
    rectangleToggle = toolsFolder->addToggle("Rectangle");
    circleToggle = toolsFolder->addToggle("Circle");
    ellipseToggle = toolsFolder->addToggle("Ellipse");
    triangleToggle = toolsFolder->addToggle("Triangle");
    rectangleToggle->setChecked(true);

    // Event listeners
    pointToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    lineToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    rectangleToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    circleToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    ellipseToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    triangleToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    outlineToggle->onToggleEvent(this, &Top_Right_GUI::onOutlineToggle);

}


//2.3
void Top_Right_GUI::onPrimitiveSelected(ofxDatGuiToggleEvent e) {
    // Deselect all other toggles when one is selected
    pointToggle->setChecked(e.target == pointToggle);
    lineToggle->setChecked(e.target == lineToggle);
    rectangleToggle->setChecked(e.target == rectangleToggle);
    circleToggle->setChecked(e.target == circleToggle);
    ellipseToggle->setChecked(e.target == ellipseToggle);
    triangleToggle->setChecked(e.target == triangleToggle);

    if (e.target == pointToggle) selectedPrimitive = PrimitiveType::POINT;
    else if (e.target == lineToggle) selectedPrimitive = PrimitiveType::LINE;
    else if (e.target == rectangleToggle) selectedPrimitive = PrimitiveType::RECTANGLE;
    else if (e.target == circleToggle) selectedPrimitive = PrimitiveType::CIRCLE;
    else if (e.target == ellipseToggle) selectedPrimitive = PrimitiveType::ELLIPSE;
    else if (e.target == triangleToggle) selectedPrimitive = PrimitiveType::TRIANGLE;

}

void Top_Right_GUI::setup(GUI* gui_manager) {
	this->gui_manager = gui_manager;
    counter = 0;
}

void Top_Right_GUI::addObjectToggle(Object3D* object)
{
    counter++;
    ofxDatGuiButton* newObject = new ofxDatGuiButton("Object " +ofToString(counter));
    newObject = objectsFolder->addButton("Object " + ofToString(counter));
    newObject->onButtonEvent([this,object,newObject](ofxDatGuiButtonEvent e) {
        object->setSelected(!object->getSelected());
        gui_manager->getScene()->resetSelection();
        gui_manager->getScene()->update_Attributes();
        if (object->getSelected())
        {
            lastObjectButton = newObject;
            gui_manager->getScene()->selectedObjects.push_back(object);
            gui_manager->getScene()->update_Attributes();
        }
        else
            lastObjectButton = nullptr;
        });
    objectsFolder->expand();
}

void Top_Right_GUI::onOutlineToggle(ofxDatGuiToggleEvent e) {
    outlineEnabled = e.checked; 
}