#include "Top_Right_GUI.h"


Top_Right_GUI::Top_Right_GUI()
{
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    gui->setWidth(200);

    toolsFolder = gui->addFolder("Drawing Tools", ofColor::white);
    toolsFolder->expand();

    backgroundColorPicker = toolsFolder->addColorPicker("Background", ofColor(31));
    strokeColorPicker = toolsFolder->addColorPicker("Outline", ofColor(255));
    fillColorPicker = toolsFolder->addColorPicker("Fill", ofColor(127, 127, 127, 127));
    lineWidthSlider = toolsFolder->addSlider("Line Width", 0.5f, 10.0f, 2.0f);

    //2.3
    pointToggle = toolsFolder->addToggle("Point");
    lineToggle = toolsFolder->addToggle("Line");
    rectangleToggle = toolsFolder->addToggle("Rectangle");
    circleToggle = toolsFolder->addToggle("Circle");
    ellipseToggle = toolsFolder->addToggle("Ellipse");
    rectangleToggle->setChecked(true);

    // Event listeners
    pointToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    lineToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    rectangleToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    circleToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);
    ellipseToggle->onToggleEvent(this, &Top_Right_GUI::onPrimitiveSelected);

}


//2.3
void Top_Right_GUI::onPrimitiveSelected(ofxDatGuiToggleEvent e) {
    // Deselect all other toggles when one is selected
    if (e.target == pointToggle) {
        pointToggle->setChecked(true);
        lineToggle->setChecked(false);
        rectangleToggle->setChecked(false);
        circleToggle->setChecked(false);
        ellipseToggle->setChecked(false);
        selectedPrimitive = PrimitiveType::POINT;
    }
    else if (e.target == lineToggle) {
        pointToggle->setChecked(false);
        lineToggle->setChecked(true);
        rectangleToggle->setChecked(false);
        circleToggle->setChecked(false);
        ellipseToggle->setChecked(false);
        selectedPrimitive = PrimitiveType::LINE;
    }
    else if (e.target == rectangleToggle) {
        pointToggle->setChecked(false);
        lineToggle->setChecked(false);
        rectangleToggle->setChecked(true);
        circleToggle->setChecked(false);
        ellipseToggle->setChecked(false);
        selectedPrimitive = PrimitiveType::RECTANGLE;
    }
    else if (e.target == circleToggle) {
        pointToggle->setChecked(false);
        lineToggle->setChecked(false);
        rectangleToggle->setChecked(false);
        circleToggle->setChecked(true);
        ellipseToggle->setChecked(false);
        selectedPrimitive = PrimitiveType::CIRCLE;
    }
    else if (e.target == ellipseToggle) {
        pointToggle->setChecked(false);
        lineToggle->setChecked(false);
        rectangleToggle->setChecked(false);
        circleToggle->setChecked(false);
        ellipseToggle->setChecked(true);
        selectedPrimitive = PrimitiveType::ELLIPSE;
    }
}