#pragma once
#include "ofMain.h"
#include "modules/Configuration/Configuration.h"

class Object3D {
public:
    Object3D();
    Object3D(const Object3D& instance);
    virtual ~Object3D();

    virtual void setup();
    virtual void update(float dt);
    virtual void draw();

    virtual void drawBoundingBox();

    // Getters
    ofVec3f getPosition() const;
    ofVec3f getRotation() const;
    ofVec3f getScale() const;
    ofColor getColor() const;
    float getOpacity() const;
    bool getSelected() const;

    // Setters
    void setPosition(const ofVec3f& position);
    void setRotation(const ofVec3f& rotation);
    void setScale(const ofVec3f& scale);
    void setColor(const ofColor& color);
    void setOpacity(float opacity);
    void setSelected(bool selected);

    // Transform methods
    void transformPosition(const ofVec3f& delta);
    void transformRotation(const ofVec3f& delta);
    void transformScale(const ofVec3f& delta);

    virtual Object3D* copy() const;

    virtual ofRectangle getScreenBoundingBox(ofCamera* cam);

    virtual void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const;

    ofColor strokeColor;
    ofColor fillColor;
    float lineWidth;

protected:
    void initializeDrawingTools();

    ofVec3f position;
    ofVec3f rotation;
    ofVec3f scale;
    ofColor color;
    float opacity;
    bool selected;
};
