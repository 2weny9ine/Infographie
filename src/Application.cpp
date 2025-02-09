#include "Application.h"
#include <algorithm>


void Application::setup() {
    ofSetWindowTitle("Infographie");
    ofLog() << "Application démarre...";

    is_key_press_up = false;
    is_key_press_down = false;
    is_key_press_right = false;
    is_key_press_left = false;

    scene.setup();
}

void Application::update() {
    time_current = ofGetElapsedTimef();
    time_elapsed = time_current - time_last;
    time_last = time_current;

    //déplacement sur le plan XZ
    if (is_key_press_up)
        scene.offset_z += scene.delta_z * time_elapsed;
    if (is_key_press_down)
        scene.offset_z -= scene.delta_z * time_elapsed;
    if (is_key_press_left)
        scene.offset_x += scene.delta_x * time_elapsed;
    if (is_key_press_right)
        scene.offset_x -= scene.delta_x * time_elapsed;

    scene.update();
}

void Application::draw() {
    img.showImage();
    scene.draw();
}



void Application::windowResized(int w, int h) {
    ofLog() << "Fenêtre redimensionnée : " << w << "x" << h;
}

void Application::keyPressed(int key)
{
    switch (key)
    {
    case OF_KEY_LEFT:
        is_key_press_left = true;
        break;
    case OF_KEY_UP:
        is_key_press_up = true;
        break;
    case OF_KEY_RIGHT:
        is_key_press_right = true;
        break;
    case OF_KEY_DOWN:
        is_key_press_down = true;
        break;
    default:
        break;
    }
}

void Application::keyReleased(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
            is_key_press_left = false;
            break;
        case OF_KEY_UP:
            is_key_press_up = false;
            break;
        case OF_KEY_RIGHT:
            is_key_press_right = false;
            break;
        case OF_KEY_DOWN:
            is_key_press_down = false;
            break;
        case ' ':
            img.imageExport("exportImage", "png");
            break;
        default:
            break;
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
