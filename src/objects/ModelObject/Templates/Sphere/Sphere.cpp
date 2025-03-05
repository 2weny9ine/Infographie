#include "Sphere.h"

Sphere::Sphere() {
    ofLogNotice("Sphere::Sphere") << "Sphere constructor called.";
    setup();
}

Sphere::~Sphere() {
    ofLogNotice("Sphere::~Sphere") << "Sphere destructor called.";
}

void Sphere::setup() {
    int resolution = 16;
    sphere.set(10.0, resolution);
    sphere.setPosition(0, 0, 0);
}

void Sphere::draw() {
    ofPushMatrix();

    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale);

    ofSetColor(color, opacity * 255);
    sphere.draw();

    if (selected) {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void Sphere::drawBoundingBox()
{
    ofSetColor(255);

    glEnable(GL_CULL_FACE); 
    glCullFace(GL_FRONT);    
    sphere.drawWireframe();   
    glDisable(GL_CULL_FACE);
}
