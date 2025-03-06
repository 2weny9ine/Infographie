#pragma once
#include "objects/Object3D.h"
#include "ofxAssimpModelLoader.h"

class ModelObject : public Object3D {
public:
    ModelObject();
    ModelObject(const ModelObject& instance);
    virtual ~ModelObject();

    bool loadModel(const std::string& path);
    std::string getModelPath() const;

    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void drawBoundingBox() override;
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam) override;

    virtual ModelObject* copy() const override;

private:
    ofxAssimpModelLoader model;
    std::string modelPath;
};
