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
    ofNoFill();
    ofSetColor(strokeColor);
    ofSetLineWidth(lineWidth);
    ofDrawBox(position, 100);
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

// Getters
ofVec3f Object3D::getPosition() const
{
    return position;
}

ofVec3f Object3D::getRotation() const
{
    return rotation;
}

ofVec3f Object3D::getScale() const
{
    return scale;
}

ofColor Object3D::getColor() const
{
    return color;
}

float Object3D::getOpacity() const
{
    return opacity;
}

bool Object3D::getSelected() const
{
    return selected;
}

// Setters
void Object3D::setPosition(const ofVec3f& position)
{
    this->position = position;
}

void Object3D::setRotation(const ofVec3f& rotation)
{
    this->rotation = rotation;
}

void Object3D::setScale(const ofVec3f& scale)
{
    this->scale = scale;
}

void Object3D::setColor(const ofColor& color)
{
    this->color = color;
}

void Object3D::setOpacity(float opacity)
{
    this->opacity = opacity;
}

void Object3D::setSelected(bool selected)
{
    this->selected = selected;
}

// Transform methods
void Object3D::transformPosition(const ofVec3f& delta)
{
    position += delta;
}

void Object3D::transformRotation(const ofVec3f& delta)
{
    rotation += delta;
}

void Object3D::transformScale(const ofVec3f& delta)
{
    scale += delta;
}
