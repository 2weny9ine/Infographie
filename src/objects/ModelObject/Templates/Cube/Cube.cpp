#include "Cube.h"

Cube::Cube() {
    ofLogNotice("Cube::Cube") << "Cube constructor called.";
    setup();
}

Cube::~Cube() {
    ofLogNotice("Cube::~Cube") << "Cube destructor called.";
}

void Cube::setup() {
    cube.set(10.0, 10.0, 10.0);
    cube.setPosition(0, 0, 0);
}

void Cube::draw() {
    ofPushMatrix();

    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale);

    ofSetColor(255);
    cube.draw();

    if (selected) {
        drawBoundingBox();
    }

    ofPopMatrix();
}
