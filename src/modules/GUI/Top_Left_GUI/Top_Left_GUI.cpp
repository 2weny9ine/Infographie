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

    duplicateInstanceButton = attributes_folder->addButton("Duplicate Instance");

    for (ofxDatGuiTextInput* input : inputs)
    {
        input->setInputType(ofxDatGuiInputType::NUMERIC);
        input->onTextInputEvent([this](ofxDatGuiTextInputEvent e) {
            SendTransformations();
            });
    }

    
    gui->addFRM();
    

    histogram_folder = gui->addFolder("Image", ofColor::white);

    deleteImagesButton = histogram_folder->addButton("Supprimer toutes les images");
    deleteImagesButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        if (imagePtr) {
            imagePtr->clearImages();
        }
    });

    duplicateInstanceButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        gui_manager->getScene()->duplicateSelectedInstances();
    });

    histogramButton = histogram_folder->addButton("Afficher l'histogramme de la première image");
    histogramButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        showHistogram = !showHistogram;
        if (showHistogram) {
            histogramButton->setLabel("Masquer histogramme");
        } else {
            histogramButton->setLabel("Afficher l'histogramme de la première image");
        }
    });
    showHistogram = false;

    exportDurationSlider = histogram_folder->addSlider("Durée Export", 1, 10, 5);
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

void Top_Left_GUI::resetTransformations()
{
    for (ofxDatGuiTextInput* input : inputs)
    {
        input->setText("--");
    }
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

void Top_Left_GUI::SendTransformations()
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

bool Top_Left_GUI::histogramEnabled() const {
    return showHistogram;
}

