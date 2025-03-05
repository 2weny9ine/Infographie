#include "Object3D.h"

Object3D::Object3D() : selected(false), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1) { 
    initializeDrawingTools(); // 2.2 /**************************************************************************/
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
    strokeColor = ofColor(255);      // White outline
    fillColor = ofColor(127, 127, 127, 127); // Semi-transparent gray fill
    lineWidth = 2.0f;                // Default thickness
}
/**************************************************************************/
/**************************************************************************/
