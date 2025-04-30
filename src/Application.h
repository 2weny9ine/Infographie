#pragma once
#include "ofMain.h"
#include "Renderer.h"
#include "modules/Scene/Scene.h"
#include "modules/Image/Image.h"
#include "modules/IlluminationClassique/IlluminationClassique.h"
#include "modules/User_Camera_Movement/User_Camera_Movement.h"
#include "modules/GUI/GUI.h"
#include "events/MouseEventHandler/MouseEventHandler.h"
#include "events/WindowResizedEventHandler/WindowResizedEventHandler.h"
#include "events/KeyEventHandler/KeyEventHandler.h"
#include "events/DragEventHandler/DragEventHandler.h"
#include "modules/Texture/TextureManager.h"


class Application : public ofBaseApp {
public:
    GUI gui;
    Scene scene;
    Renderer renderer;
    
    User_Camera_Movement user_camera_movement;

    ofFbo sceneFbo;
    ofPixels fboPixels;

    float time_current, time_elapsed, time_last;
    bool isDrawingMode;
    ofColor backgroundColor;

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

    // Getters and Setters
    GUI& getGui();
    Scene& getScene();
    Renderer& getRenderer();
    User_Camera_Movement& getUserCameraMovement();
    ofFbo& getSceneFbo();
    ofPixels& getFboPixels();
    float getTimeCurrent() const;
    float getTimeElapsed() const;
    float getTimeLast() const;
    bool getIsDrawingMode() const;
    ofColor getBackgroundColor() const;
   


    void setScene(const Scene& scene);
    void setRenderer(const Renderer& renderer);
    void setUserCameraMovement(const User_Camera_Movement& user_camera_movement);
    void setSceneFbo(const ofFbo& sceneFbo);
    void setFboPixels(const ofPixels& fboPixels);
    void setTimeCurrent(float time_current);
    void setTimeElapsed(float time_elapsed);
    void setTimeLast(float time_last);
    void setIsDrawingMode(bool isDrawingMode);
    void setBackgroundColor(const ofColor& backgroundColor);

    static Application& getInstance();
    
    IlluminationClassique* getIllumination() { return illuminationClassique; }
    TextureManager& getTextureManager() { return TextureManager::get(); } //texture
private:
    Application();
    ~Application();

    MouseEventHandler mouseEventHandler;
    WindowResizedEventHandler windowResizedHandler;
    KeyEventHandler keyEventHandler;
    DragEventHandler dragEventHandler;
    IlluminationClassique* illuminationClassique;
};
