#pragma once
#include "ofMain.h"
#include "Renderer.h"
#include "modules/Image/Image.h"

class Application : public ofBaseApp {
public:
    Image img;
    Renderer rend;
    bool is_key_press_up, is_key_press_down, is_key_press_left, is_key_press_right;
    float time_current, time_elapsed, time_last;
    
    void setup();
    void draw();
    
    void update();
    
    void keyReleased(int key);
    
    void windowResized(int w, int h);

    void keyPressed(int key);
    
    void dragEvent(ofDragInfo dragInfo);
    
    void updateWindowSize();
    
    void exit();
};
