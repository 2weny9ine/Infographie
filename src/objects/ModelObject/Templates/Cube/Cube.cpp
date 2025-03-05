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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);    
    cube.drawWireframe();  
    glDisable(GL_CULL_FACE);
}
