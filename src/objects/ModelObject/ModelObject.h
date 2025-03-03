#pragma once
#include "objects/Object3D.h"
#include "ofxAssimpModelLoader.h"

class ModelObject : public Object3D {
public:
    ModelObject();
    virtual ~ModelObject();

    bool loadModel(const std::string& path);

    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;

    virtual void drawBoundingBox() override;
    //yacine
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam) override;

private:
    ofxAssimpModelLoader model;
};
