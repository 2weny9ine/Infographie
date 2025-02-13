#include "ModelObject.h"

ModelObject::ModelObject() {}

ModelObject::~ModelObject() {}

bool ModelObject::loadModel(const std::string& path) {
    return model.loadModel(path, true);
}

void ModelObject::setup() {

}

void ModelObject::update(float dt) {

}

void ModelObject::draw() {
    ofPushMatrix();

    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale);

    model.drawFaces();

    if (selected) {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void ModelObject::drawBoundingBox() {
    ofNoFill();
    ofSetColor(0, 255, 0);

    ofBoxPrimitive box;
    box.set(model.getSceneMax().x - model.getSceneMin().x,
        model.getSceneMax().y - model.getSceneMin().y,
        model.getSceneMax().z - model.getSceneMin().z);
    box.setPosition((model.getSceneMin().x + model.getSceneMax().x) / 2,
        (model.getSceneMin().y + model.getSceneMax().y) / 2,
        (model.getSceneMin().z + model.getSceneMax().z) / 2);
    box.drawWireframe();
    ofSetColor(255);
}
