#include "Top_Left_GUI.h"
#include "modules/GUI/GUI.h"

Top_Left_GUI::Top_Left_GUI()
{
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    
    camPosition_folder = gui->addFolder("Camera Position", ofColor::white);
    
    xCam_position = camPosition_folder->addTextInput("X", "N/A");
    xCam_position->setInputType(ofxDatGuiInputType::NUMERIC);
    
    yCam_position = camPosition_folder->addTextInput("Y", "N/A");
    yCam_position->setInputType(ofxDatGuiInputType::NUMERIC);
    
    zCam_position = camPosition_folder->addTextInput("Z", "N/A");
    zCam_position->setInputType(ofxDatGuiInputType::NUMERIC);

    
    gui->addFRM();
    

    histogram_folder = gui->addFolder("Image", ofColor::white);

 

    histogramButton = histogram_folder->addButton("Afficher l'histogramme");
    histogramButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        showHistogram = !showHistogram;
        if (showHistogram) {
            histogramButton->setLabel("Masquer histogramme");
        } else {
            histogramButton->setLabel("Afficher l'histogramme");
        }
    });
    showHistogram = false;

    exportDurationSlider = histogram_folder->addSlider("Durée Export", 1, 20, 5);
    exportDurationSlider->setPrecision(0);
    exportButton = histogram_folder->addButton("Exporter la séquence");
    exportButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        if (imagePtr) {
            imagePtr->setExportDuration(exportDurationSlider->getValue());
            imagePtr->setExportTriggered(true);
        }
    });


    color_folder = gui->addFolder("Image Color", ofColor::white);

    toggleColorButton = color_folder->addButton("Activer Couleurs");
    toggleColorButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        applyColors = !applyColors;
        if (applyColors) {
            toggleColorButton->setLabel("Désactiver Couleurs");
        } else {
            toggleColorButton->setLabel("Activer Couleurs");
        }
    });
    applyColors = false;

    colorPickerRGB = color_folder->addColorPicker("Couleur (RGB)", ofColor::white);
    hueValue = color_folder->addSlider("Hue", 0, 255, 127);
    satValue = color_folder->addSlider("Saturation", 0, 255, 127);
    briValue = color_folder->addSlider("Brightness", 0, 255, 127);

    imagePtr = nullptr;

}

void Top_Left_GUI::setup(GUI* gui_manager) {
    this->gui_manager = gui_manager;
}


ofColor Top_Left_GUI::getRGBColor() {
    return colorPickerRGB->getColor();
}

ofColor Top_Left_GUI::getHSBColor() {
    float hValue = hueValue->getValue();
    float sValue = satValue->getValue();
    float bValue = briValue->getValue();
    ofColor c;
    c.setHsb(hValue, sValue, bValue);
    return c;
}

ofxDatGui* Top_Left_GUI::getGui()
{
    return gui;
}

bool Top_Left_GUI::histogramEnabled() const {
    return showHistogram;
}

