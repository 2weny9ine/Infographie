#include "Application.h"
#include <algorithm>


void Application::setup() {
    ofSetWindowTitle("Infographie");
    ofLog() << "Application démarre...";
    ofBackground(50, 50, 50);
}

void Application::update() {

}

void Application::draw() {
    img.showImage();
}



void Application::windowResized(int w, int h) {
    ofLog() << "Fenêtre redimensionnée : " << w << "x" << h;
}

void Application::keyReleased(int key)
{
    if (key == ' ') {
        img.imageExport("exportImage", "png");
       }
}

void Application::dragEvent(ofDragInfo dragInfo) {
    if (!dragInfo.files.empty()) {
        for (const auto& file : dragInfo.files) {
            img.importImage(file);
        }
        
    }
}


void Application::exit()
{
  ofLog() << "<app::exit>";
}
