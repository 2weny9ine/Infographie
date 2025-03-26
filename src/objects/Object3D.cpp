#include "Object3D.h"

Object3D::Object3D()
    : selected(false), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
    color(ofColor::grey), opacity(1.0f)
{
    initializeDrawingTools();
}

Object3D::Object3D(const Object3D& instance)
    : selected(instance.selected), position(instance.position),
    rotation(instance.rotation), scale(instance.scale),
    color(instance.color), opacity(instance.opacity),
    strokeColor(instance.strokeColor), fillColor(instance.fillColor),
    lineWidth(instance.lineWidth)
{
    initializeDrawingTools();
}

Object3D::~Object3D() {}

void Object3D::setup() {}

void Object3D::update(float dt) {}

void Object3D::draw() {}

void Object3D::drawBoundingBox()
{
    // Default implementation does nothing.
}

Object3D* Object3D::copy() const
{
    return new Object3D(*this);
}

ofRectangle Object3D::getScreenBoundingBox(ofCamera* cam)
{
    return ofRectangle();
}

void Object3D::initializeDrawingTools()
{
    strokeColor = ofColor(255);
    fillColor = ofColor(127, 127, 127, 127);
    lineWidth = 2.0f;
}

void Object3D::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
{
    // Default: return a 1x1x1 cube centered at position.
    outMin = position - ofVec3f(0.5, 0.5, 0.5);
    outMax = position + ofVec3f(0.5, 0.5, 0.5);
}

// Getters
ofVec3f Object3D::getPosition() const { return position; }
ofVec3f Object3D::getRotation() const { return rotation; }
ofVec3f Object3D::getScale() const { return scale; }
ofColor Object3D::getColor() const { return color; }
float Object3D::getOpacity() const { return opacity; }
bool Object3D::getSelected() const { return selected; }

// Setters
void Object3D::setPosition(const ofVec3f& pos) { position = pos; }
void Object3D::setRotation(const ofVec3f& rot) { rotation = rot; }
void Object3D::setScale(const ofVec3f& s) { scale = s; }
void Object3D::setColor(const ofColor& c) { color = c; }
void Object3D::setOpacity(float op) { opacity = op; }
void Object3D::setSelected(bool sel) { selected = sel; }

// Transform methods
void Object3D::transformPosition(const ofVec3f& delta) { position += delta; }
void Object3D::transformRotation(const ofVec3f& delta) { rotation += delta; }
void Object3D::transformScale(const ofVec3f& delta) { scale += delta; }
