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
};
