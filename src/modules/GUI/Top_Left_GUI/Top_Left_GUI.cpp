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
            imagePtr->triggerExport(true);
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
    
    
    // Dossier : Matériaux
    material_folder = gui->addFolder("Matériaux", ofColor::white);

    materialEffectToggle = material_folder->addButton("Activer Effet Matériau");
    materialEffectToggle->onButtonEvent([this](ofxDatGuiButtonEvent) {
        illumination->materialEffectEnabled = !illumination->materialEffectEnabled;
        materialEffectEnabled = illumination->materialEffectEnabled;

        if (materialEffectEnabled) {
            materialEffectToggle->setLabel("Désactiver Effet Matériau");
            illumIdx = static_cast<int>(IlluminationClassique::Mode::PHONG);
        } else {
            materialEffectToggle->setLabel("Activer Effet Matériau");
            illumIdx = static_cast<int>(IlluminationClassique::Mode::LAMBERT);
        }

        illumBtn->setLabel(illumNames[illumIdx]);
        illumination->setMode(static_cast<IlluminationClassique::Mode>(illumIdx));
        ofLogNotice() << "Effet Matériau : " << (materialEffectEnabled ? "activé" : "désactivé");
    });

    materialOptions = { "Mat", "Plastique", "Métallique" };
    currentMaterialIndex = 0;

    materialButton = material_folder->addButton(materialOptions[currentMaterialIndex]);
    materialButton->onButtonEvent([this](ofxDatGuiButtonEvent) {
        currentMaterialIndex = (currentMaterialIndex + 1) % materialOptions.size();
        materialButton->setLabel(materialOptions[currentMaterialIndex]);
        ofLogNotice() << "Matériau sélectionné : " << materialOptions[currentMaterialIndex];
    });

    // Dossier : Illumination
    illumFolder = gui->addFolder("Illumination", ofColor::purple);

    illumNames = { "Off", "Lambert", "Gouraud", "Phong", "Blinn‑Phong", "Toon" };
    illumIdx = 0;

    illumBtn = illumFolder->addButton(illumNames[illumIdx]);
    illumBtn->onButtonEvent([this](ofxDatGuiButtonEvent) {
        illumIdx = (illumIdx + 1) % illumNames.size();
        illumBtn->setLabel(illumNames[illumIdx]);

        if (illumination) {
            illumination->setMode(static_cast<IlluminationClassique::Mode>(illumIdx));
            ofLogNotice() << "Mode d’illumination : " << illumNames[illumIdx];
        } else {
            ofLogError() << "Pointeur d’illumination nul.";
        }
    });
    

    // Dossier : Types de lumières
    lightsFolder = gui->addFolder("Lumières", ofColor::white);

    toggleAmbiante        = lightsFolder->addToggle("Ambiante",       false);
    toggleDirectionnelle  = lightsFolder->addToggle("Directionnelle", false);
    togglePonctuelle      = lightsFolder->addToggle("Ponctuelle",     false);
    toggleProjecteur      = lightsFolder->addToggle("Projecteur",     false);
    toggleMouseLight      = lightsFolder->addToggle("Lumière Souris", false);

    toggleAmbiante->onToggleEvent([this](ofxDatGuiToggleEvent e) {
        if (illumination) illumination->activeLightAmbient = e.checked;
        ofLogNotice() << "Ambiante : " << (e.checked ? "ON" : "OFF");
    });
    toggleDirectionnelle->onToggleEvent([this](ofxDatGuiToggleEvent e) {
        if (illumination) illumination->activeLightDirectional = e.checked;
        ofLogNotice() << "Directionnelle : " << (e.checked ? "ON" : "OFF");
    });
    togglePonctuelle->onToggleEvent([this](ofxDatGuiToggleEvent e) {
        if (illumination) illumination->activeLightPoint = e.checked;
        ofLogNotice() << "Ponctuelle : " << (e.checked ? "ON" : "OFF");
    });
    toggleProjecteur->onToggleEvent([this](ofxDatGuiToggleEvent e) {
        if (illumination) illumination->activeLightSpot = e.checked;
        ofLogNotice() << "Projecteur : " << (e.checked ? "ON" : "OFF");
    });
    toggleMouseLight->onToggleEvent([this](ofxDatGuiToggleEvent e) {
        if (illumination) illumination->activeMouseLight = e.checked;
        ofLogNotice() << "Lumière Souris : " << (e.checked ? "ON" : "OFF");
    });
    
    auto sliderGlobalIntensity = lightsFolder->addSlider("Intensité ", 0.0f, 1.0f); 
    sliderGlobalIntensity->setValue(1.0f);
    sliderGlobalIntensity->onSliderEvent([this](ofxDatGuiSliderEvent e){
        if (!illumination) return;
        float I = e.value;

     
        ofFloatColor intensityColor(I, I, I);
        
        illumination->lightDirectional.setDiffuseColor(intensityColor);
        illumination->lightPoint.setDiffuseColor(intensityColor);
        illumination->lightSpot.setDiffuseColor(intensityColor);
        illumination->lightMouse.setDiffuseColor(intensityColor);
    });
    
    auto pickerGlobalColor = lightsFolder->addColorPicker("Couleur ", ofColor::white);
    pickerGlobalColor->onColorPickerEvent([this](ofxDatGuiColorPickerEvent e){
        if (!illumination) return;
        

        ofFloatColor color(
            e.color.r / 255.0f,
            e.color.g / 255.0f,
            e.color.b / 255.0f
        );

        illumination->lightDirectional.setDiffuseColor(color);
        illumination->lightPoint.setDiffuseColor(color);
        illumination->lightSpot.setDiffuseColor(color);
        illumination->lightMouse.setDiffuseColor(color);
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
