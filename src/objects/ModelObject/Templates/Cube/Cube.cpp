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

    ofSetColor(color, opacity * 255);
    cube.draw();

    if (selected) {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void Cube::drawBoundingBox()
{
    ofSetColor(255);
    glLineWidth(2.0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    cube.drawWireframe();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0);
}