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
    ofScale(scale.x, -scale.y, scale.z);

    glDisable(GL_CULL_FACE);

    ofEnableNormalizedTexCoords();
    model.drawFaces();
    ofDisableNormalizedTexCoords();

    if (selected) {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void ModelObject::drawBoundingBox() {
    if (model.getNumMeshes() == 0) {
        ofLogError("ModelObject::drawBoundingBox") << "No meshes loaded!";
        return;
    }

    glm::vec3 sceneMin = model.getSceneMin();
    glm::vec3 sceneMax = model.getSceneMax();

    glm::mat4 modelMatrix = model.getModelMatrix();
    glm::vec3 worldMin = glm::vec3(modelMatrix * glm::vec4(sceneMin, 1.0));
    glm::vec3 worldMax = glm::vec3(modelMatrix * glm::vec4(sceneMax, 1.0));
    glm::vec3 boxSize = worldMax - worldMin;
    glm::vec3 boxCenter = (worldMin + worldMax) * 0.5f;

    ofPushMatrix();

    ofTranslate(boxCenter);

    ofNoFill();
    ofSetColor(65, 145, 221);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    ofDrawBox(glm::vec3(0, 0, 0), boxSize.x, boxSize.y, boxSize.z);

    glDisable(GL_CULL_FACE);

    ofFill();
    ofPopMatrix();
}
