#pragma once
#include "ofMain.h"
#include "Renderer.h"
#include "Image.h"

class Application : public ofBaseApp {
public:
    Image img;

    
    void setup();
    void draw();
    
    void update();
    
    void keyReleased(int key);
    
    void windowResized(int w, int h);
    
    void dragEvent(ofDragInfo dragInfo);
    
    void updateWindowSize();
    
    void exit();
};
