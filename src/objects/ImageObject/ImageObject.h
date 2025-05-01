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

    //6.3
    void enableToneMapping(bool enable) { toneMappingEnabled = enable; }
    void setToneMappingExposure(float exposure) { toneMappingExposure = exposure; }
    void setToneMappingGamma(float gamma) { toneMappingGamma = gamma; }
    virtual std::vector<Property> getProperties() const override;
    virtual void setProperty(const Property& prop) override;


private:
    ofImage image;

    //6.3
    ofShader toneMappingShader;
    bool toneMappingEnabled = false;
    float toneMappingExposure = 1.0f;
    float toneMappingGamma = 2.2f;


};
