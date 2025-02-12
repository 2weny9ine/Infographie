#pragma once
#include "ofMain.h"
#include "Renderer.h"
#include "modules/Scene/Scene.h"
#include "modules/Image/Image.h"
#include "modules/User_Camera_Movement/User_Camera_Movement.h"

class Application : public ofBaseApp {
public:
    Image img;
    Scene scene;
    User_Camera_Movement user_camera_movement;

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
