#pragma once
#include "objects/Object3D.h"

class ImageObject : public Object3D {
public:
    ImageObject();
    virtual ~ImageObject();

    bool loadImage(const std::string& path);
    void drawFilm(const ofColor& film);
    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void applyFilter(const ofColor& filter);

    ofImage& getImage() { return image; }

    void setPosition(const ofVec3f& pos)
    {
        position = pos;
    }

    bool applyUserColor;
    ofColor customUserColor;
    ofPlanePrimitive plane;

    // yacine
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam) override;
    virtual void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;
    virtual void drawBoundingBox() override;

private:
    ofImage image;
};
