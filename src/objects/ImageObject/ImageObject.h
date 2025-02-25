#pragma once
#include "objects/Object3D.h"

class ImageObject : public Object3D {
public:
    ImageObject();
    virtual ~ImageObject();
    ofVec3f position;
    bool loadImage(const std::string& path);

    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    
    virtual void drawBoundingBox() override;
    
    ofImage& getImage() { return image; }
    
    void setPosition(const ofVec3f& pos) {
        position = pos;
    }

private:
    ofImage image;
    ofPlanePrimitive plane;
};
