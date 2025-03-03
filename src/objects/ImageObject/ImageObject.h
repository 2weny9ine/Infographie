#pragma once
#include "objects/Object3D.h"

class ImageObject : public Object3D {
public:
    ImageObject();
    virtual ~ImageObject();

    bool loadImage(const std::string& path);

    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;

    virtual void drawBoundingBox() override;

    //yacine
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam) override;

private:
    ofImage image;
    ofPlanePrimitive plane;
};
