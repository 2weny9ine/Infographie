#include "Top_Left_GUI.h"
#include "modules/GUI/GUI.h"

Top_Left_GUI::Top_Left_GUI()
{
	gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	
	position_folder = gui->addFolder("Camera Position", ofColor::white);
	position_folder->expand();

	x_position = position_folder->addTextInput("X", "N/A");
	x_position->setInputType(ofxDatGuiInputType::NUMERIC);

	y_position = position_folder->addTextInput("Y", "N/A");
	y_position->setInputType(ofxDatGuiInputType::NUMERIC);

	z_position = position_folder->addTextInput("Z", "N/A");
	z_position->setInputType(ofxDatGuiInputType::NUMERIC);
	
	gui->addFRM();

    histogram_folder = gui->addFolder("Image", ofColor::white);
    deleteImagesButton = histogram_folder->addButton("Supprimer toutes les images");
       deleteImagesButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
           if (imagePtr) {
               imagePtr->clearImages();
           }
       });
    histogramButton = histogram_folder->addButton("Afficher l'histogramme de la première image");
    
    histogramButton->onButtonEvent([this](ofxDatGuiButtonEvent e) {
        showHistogram = !showHistogram;
        
        if(showHistogram){
            histogramButton->setLabel("Masquer histogramme");
        } else {
            histogramButton->setLabel("Afficher l'histogramme de la première image");
        }
    });
    showHistogram= false;
    
    
   
    imagePtr = nullptr;
}

void Top_Left_GUI::setup(GUI* gui_manager) {
	this->gui_manager = gui_manager;
}