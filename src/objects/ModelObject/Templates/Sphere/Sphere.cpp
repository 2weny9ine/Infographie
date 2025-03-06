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

void Sphere::drawBoundingBox() {
    ofSetColor(255);
    glLineWidth(2.0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    sphere.drawWireframe();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0);
}