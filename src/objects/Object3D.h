#pragma once
#include "ofMain.h"
#include "modules/Properties/ModularProperties.h"

class Object3D : public IPropertyProvider {
public:
    Object3D();
    Object3D(const Object3D& instance);
    virtual ~Object3D();

    virtual void setup();
    virtual void update(float dt);
    virtual void draw();
    virtual void drawBoundingBox();

    ofVec3f getPosition() const;
    ofVec3f getRotation() const;
    ofVec3f getScale() const;
    ofColor getColor() const;
    float getOpacity() const;
    bool getSelected() const;

    void setPosition(const ofVec3f& position);
    void setRotation(const ofVec3f& rotation);
    void setScale(const ofVec3f& scale);
    void setColor(const ofColor& color);
    void setOpacity(float opacity);
    void setSelected(bool selected);

    void transformPosition(const ofVec3f& delta);
    void transformRotation(const ofVec3f& delta);
    void transformScale(const ofVec3f& delta);

    virtual Object3D* copy() const;
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam);
    virtual void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const;

    virtual std::vector<Property> getProperties() const override;
    virtual void setProperty(const Property& prop) override;

protected:
    void initializeDrawingTools();

    ofVec3f position, rotation, scale;
    ofColor color;
    float opacity;
    bool selected;

    ofColor strokeColor, fillColor;
    float lineWidth;
};
