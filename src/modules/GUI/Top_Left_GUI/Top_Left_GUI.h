#pragma once
#include "ofxDatGui.h"
#include "Image.h"
//#include "modules/IlluminationClassique/IlluminationClassique.h"

class IlluminationClassique;
class GUI;

class Top_Left_GUI
{
public:
    Top_Left_GUI();
    void update();
    
    void setImage(Image* img) { imagePtr = img; }
    
    void setup(GUI* gui_manager);
    bool colorFilterEnabled() const { return applyColors; }
    bool histogramEnabled() const;
    
    int getX() const { return gui->getPosition().x; }
    int getY() const { return gui->getPosition().y; }
    int getHeight() const { return gui->getHeight(); }
    
    ofColor getRGBColor();
    ofColor getHSBColor();
    
    ofxDatGui* getGui();
    
    bool isMouseOverGui(int x, int y) const;
    int getCurrentMaterialIndex() const { return currentMaterialIndex; }
    bool isMaterialEffectEnabled() const { return materialEffectEnabled; }
    
    
    void setIlluminationPtr(IlluminationClassique* ptr) { illumination = ptr; }
    
    
    
    ofxDatGuiToggle* toggleAmbiante;
    ofxDatGuiToggle* toggleDirectionnelle;
    ofxDatGuiToggle* togglePonctuelle;
    ofxDatGuiToggle* toggleProjecteur;
    ofxDatGuiToggle* toggleMouseLight;
    
    
    bool isAmbianteOn()       const { return toggleAmbiante      && toggleAmbiante->getEnabled(); }
    bool isDirectionnelleOn() const { return toggleDirectionnelle&& toggleDirectionnelle->getEnabled(); }
    bool isPonctuelleOn()     const { return togglePonctuelle    && togglePonctuelle->getEnabled(); }
    bool isProjecteurOn()     const { return toggleProjecteur    && toggleProjecteur->getEnabled(); }
    void setIllumination(IlluminationClassique* ptr) {
        illumination = ptr;
    }
    
private:
    GUI* gui_manager;
    ofxDatGui* gui;
    
    
    ofxDatGuiFolder* camPosition_folder;
    ofxDatGuiTextInput* xCam_position;
    ofxDatGuiTextInput* yCam_position;
    ofxDatGuiTextInput* zCam_position;
    
    ofxDatGuiFolder* create_folder;
    vector<string> shape_options;
    ofxDatGuiDropdown* shape_dropdown;
    ofxDatGuiSlider* exportDurationSlider;
    ofxDatGuiButton* exportButton;
    
    ofxDatGuiFolder* histogram_folder;
    ofxDatGuiButton* histogramButton;
    ofxDatGuiButton* deleteImagesButton;
    bool showHistogram;
    
    ofxDatGuiFolder* color_folder;
    ofxDatGuiColorPicker* colorPickerRGB;
    ofxDatGuiSlider* hueValue;
    ofxDatGuiSlider* satValue;
    ofxDatGuiSlider* briValue;
    ofxDatGuiButton* toggleColorButton;
    bool applyColors;
    Image* imagePtr;
    
    ofxDatGuiFolder* material_folder;
    ofxDatGuiButton* materialButton;
    ofxDatGuiButton* materialEffectToggle;
    bool materialEffectEnabled;
    vector<string> materialOptions;
    int currentMaterialIndex;
    
    
    IlluminationClassique* illumination;
    ofxDatGuiFolder* illumFolder;
    ofxDatGuiButton* illumBtn;
    int illumIdx;
    std::vector<std::string> illumNames;
    
    
    
    ofxDatGuiFolder* lightsFolder;
};
