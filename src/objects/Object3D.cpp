#include "Object3D.h"

Object3D::Object3D() : selected(false), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1), color(ofColor::grey), opacity(1.0f) {}

Object3D::~Object3D() {}

void Object3D::setup() {

}

void Object3D::update(float dt) {

}

void Object3D::draw() {

}

void Object3D::drawBoundingBox() {

}

void Object3D::setColor(ofColor color) {
	this->color = color;
}

void Object3D::setScale(ofVec3f scale) {
	this->scale = scale;
}

void Object3D::setOpacity(float opacity) {
	this->opacity = opacity;
}