#pragma once
#include "ofMain.h"
#include "Renderer.h"
#include "modules/Scene/Scene.h"
#include "modules/Image/Image.h"
#include "modules/User_Camera_Movement/User_Camera_Movement.h"
#include "modules/GUI/GUI.h"

#include "events/DragEvent/DragEvent.h"

class Application : public ofBaseApp {
public:
    GUI gui;
    Scene scene;
    Renderer renderer; //2.2 /**************************************************************************/
    User_Camera_Movement user_camera_movement;

    
    ofFbo sceneFbo;
    ofPixels fboPixels;


    float time_current, time_elapsed, time_last;

    /**********************************************************/
    bool isDrawingMode = false;
    
    void setup();
    void draw();
    void update();
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void keyPressed(int key);  
    void dragEvent(ofDragInfo dragInfo);     
    void exit();

private:
    ofColor backgroundColor; //2.2 /**************************************************************************/
};
