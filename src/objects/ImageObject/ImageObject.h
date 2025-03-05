#pragma once
#include "objects/Object3D.h"

class ImageObject : public Object3D {
public:
    ImageObject();
    virtual ~ImageObject();

    bool loadImage(const std::string& path);
    void drawFilm(const ofColor &film);
    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    void applyFilter(const ofColor &filter);
    virtual void drawBoundingBox() override;
    
    ofImage& getImage() { return image; }
    
    void setPosition(const ofVec3f& pos) {
        position = pos;
    }
    
    static bool applyUserColor;
    static ofColor customUserColor;
    ofPlanePrimitive plane;
    
private:
    ofImage image;

};
