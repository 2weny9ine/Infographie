#include "Object3D.h"

Object3D::Object3D() : selected(false), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), color(ofColor::grey), opacity(1.0f) {
    initializeDrawingTools();
}


Object3D::~Object3D() {}

void Object3D::setup() {

}

void Object3D::update(float dt) {

}

void Object3D::draw() {

}

void Object3D::drawBoundingBox() {
    /**************************************************************************/
    ofNoFill(); // 2.2
    ofSetColor(strokeColor); // 2.2
    ofSetLineWidth(lineWidth);  // 2.2
    /**************************************************************************/
    ofDrawBox(position, 100);

}

/**************************************************************************/
/**************************************************************************/
//yacine
ofRectangle Object3D::getScreenBoundingBox(ofCamera* cam) {
    return ofRectangle();
}

// 2.2
void Object3D::initializeDrawingTools() {
    strokeColor = ofColor(255);
    fillColor = ofColor(127, 127, 127, 127); 
    lineWidth = 2.0f;   
}
/**************************************************************************/
/**************************************************************************/

void Object3D::setColor(ofColor color) {
	this->color = color;
}

void Object3D::setScale(ofVec3f scale) {
	this->scale = scale;
}

void Object3D::setOpacity(float opacity) {
	this->opacity = opacity;
}