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

    ofSetColor(fillColor); // Task 2.2: Apply fill color /**************************************************************************/
    model.drawFaces();

    if (selected) {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void ModelObject::drawBoundingBox() {
    ofNoFill();
    ofSetColor(strokeColor); // Task 2.2: Use strokeColor
    ofSetLineWidth(lineWidth); // Task 2.2: Use lineWidth
    ofBoxPrimitive box;
    box.set(model.getSceneMax().x - model.getSceneMin().x,
        model.getSceneMax().y - model.getSceneMin().y,
        model.getSceneMax().z - model.getSceneMin().z);
    box.setPosition((model.getSceneMin().x + model.getSceneMax().x) / 2,
        (model.getSceneMin().y + model.getSceneMax().y) / 2,
        (model.getSceneMin().z + model.getSceneMax().z) / 2);
    box.drawWireframe();
}

/**************************************************************************/
/**************************************************************************/
//yacine
ofRectangle ModelObject::getScreenBoundingBox(ofCamera* cam) {
    glm::vec3 min = model.getSceneMin();
    glm::vec3 max = model.getSceneMax();

    glm::vec3 corners[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {min.x, max.y, min.z}, {max.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {min.x, max.y, max.z}, {max.x, max.y, max.z}
    };

    float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;
    for (auto& corner : corners) {
        glm::vec3 screenPos = cam->worldToScreen(corner + position);
        minX = std::min(minX, screenPos.x);
        maxX = std::max(maxX, screenPos.x);
        minY = std::min(minY, screenPos.y);
        maxY = std::max(maxY, screenPos.y);
    }
    return ofRectangle(minX, minY, maxX - minX, maxY - minY);
}
/**************************************************************************/
/**************************************************************************/

