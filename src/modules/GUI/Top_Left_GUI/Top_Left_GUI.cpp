#include "Top_Left_GUI.h"
#include "modules/GUI/GUI.h"

#include "Application.h"
#include "modules/IlluminationClassique/IlluminationClassique.h"

Top_Left_GUI::Top_Left_GUI()
{
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    
    camPosition_folder = new ofxDatGuiFolder("Camera Position", ofColor::white);
    
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
    
    
    material_folder = gui->addFolder("Materials", ofColor::white);
    
    materialEffectToggle = material_folder->addButton("Activer Effet Matériau");
    materialEffectEnabled = false;
    materialEffectToggle->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        materialEffectEnabled = !materialEffectEnabled;
        if(materialEffectEnabled) {
            materialEffectToggle->setLabel("Désactiver Effet Matériau");
        } else {
            materialEffectToggle->setLabel("Activer Effet Matériau");
        }
        ofLogNotice() << "Effet Matériau activé : " << materialEffectEnabled;
    });
    
    materialOptions = {"Mat", "Plastique", "Métallique"};
    currentMaterialIndex = 0;
    materialButton = material_folder->addButton(materialOptions[currentMaterialIndex]);
    materialButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        currentMaterialIndex = (currentMaterialIndex + 1) % materialOptions.size();
        materialButton->setLabel(materialOptions[currentMaterialIndex]);
        ofLogNotice() << "Matériau sélectionné : " << materialOptions[currentMaterialIndex];
    });
    
    
    
    illumFolder = gui->addFolder("Illumination", ofColor::white);
    
    illumNames = { "Off", "Lambert", "Gouraud", "Phong", "Blinn‑Phong" };
    illumIdx   = 0;
    illumBtn   = illumFolder->addButton(illumNames[illumIdx]);
    
    illumBtn->onButtonEvent([this](ofxDatGuiButtonEvent)
    {
        illumIdx = (illumIdx + 1) % illumNames.size();
        illumBtn->setLabel(illumNames[illumIdx]);

        if(illumination){                       
            illumination->setMode(
               static_cast<IlluminationClassique::Mode>(illumIdx));
            ofLogNotice() << "GUI set mode = " << illumIdx;   
        } else {
            ofLogError() << "illumination ptr is null in GUI";
        }
    });

    
    
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

bool Top_Left_GUI::isMouseOverGui(int x, int y) const
{
    if (!gui) return false;
    
    float guiX = gui->getPosition().x;
    float guiY = gui->getPosition().y;
    float guiWidth = gui->getWidth();
    float guiHeight = gui->getHeight();
    
    return (x >= guiX && x <= guiX + guiWidth &&
            y >= guiY && y <= guiY + guiHeight);
}
