#pragma once
#include "ofMain.h"

class Object3D {
public:
    Object3D();
    virtual ~Object3D();

    virtual void setup();
    virtual void update(float dt);
    virtual void draw();

    ofVec3f position;
    ofVec3f rotation;
    ofVec3f scale;

    bool selected;

    virtual void drawBoundingBox();

    //yacine
    /**************************************************************************/
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam) ;

    // Task 2.2
    ofColor strokeColor;  // 2.2
    ofColor fillColor;    // 2.2
    float lineWidth;      // 2.2
    /**************************************************************************/
    /**************************************************************************/

protected:
    void initializeDrawingTools(); // 2.2 /**************************************************************************/
};
