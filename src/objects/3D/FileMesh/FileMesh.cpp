#include "FileMesh.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

FileMesh::FileMesh() : modelPath("") {}

FileMesh::FileMesh(const FileMesh& other)
    : Object3D(other), modelPath(other.modelPath)
{
    loadModel(modelPath);
}

FileMesh::~FileMesh() {}

bool FileMesh::loadModel(const std::string& path)
{
    modelPath = path;
    return model.loadModel(path, true);
}

std::string FileMesh::getModelPath() const
{
    return modelPath;
}

void FileMesh::setup() {}

void FileMesh::update(float dt) {}

void FileMesh::draw()
{
    ofPushMatrix();

    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale.x, -scale.y, scale.z);

    ofSetColor(fillColor);

    glDisable(GL_CULL_FACE);
    ofEnableNormalizedTexCoords();

    model.drawFaces();
    ofDisableNormalizedTexCoords();

    if (selected)
    {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void FileMesh::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
{
    if (const_cast<ofxAssimpModelLoader&>(model).getNumMeshes() == 0)
    {
        outMin = position;
        outMax = position;
        return;
    }

    glm::vec3 sceneMin = const_cast<ofxAssimpModelLoader&>(model).getSceneMin();
    glm::vec3 sceneMax = const_cast<ofxAssimpModelLoader&>(model).getSceneMax();

    ofMatrix4x4 T, R, S;
    T.makeTranslationMatrix(position.x, position.y, position.z);
    R.makeRotationMatrix(rotation.x, ofVec3f(1, 0, 0));
    R *= ofMatrix4x4::newRotationMatrix(rotation.y, ofVec3f(0, 1, 0));
    R *= ofMatrix4x4::newRotationMatrix(rotation.z, ofVec3f(0, 0, 1));
    S.makeScaleMatrix(scale.x, -scale.y, scale.z);
    ofMatrix4x4 fileMeshTransform = T * R * S;

    ofMatrix4x4 modelMat = const_cast<ofxAssimpModelLoader&>(model).getModelMatrix();

    ofMatrix4x4 composite = fileMeshTransform * modelMat;

    glm::mat4 M = glm::make_mat4(composite.getPtr());

    glm::vec3 corners[8] = {
        { sceneMin.x, sceneMin.y, sceneMin.z },
        { sceneMax.x, sceneMin.y, sceneMin.z },
        { sceneMin.x, sceneMax.y, sceneMin.z },
        { sceneMax.x, sceneMax.y, sceneMin.z },
        { sceneMin.x, sceneMin.y, sceneMax.z },
        { sceneMax.x, sceneMin.y, sceneMax.z },
        { sceneMin.x, sceneMax.y, sceneMax.z },
        { sceneMax.x, sceneMax.y, sceneMax.z }
    };

    glm::vec3 actualMin(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 actualMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (auto& c : corners)
    {
        glm::vec4 worldCorner4 = M * glm::vec4(c, 1.0f);
        glm::vec3 worldCorner(worldCorner4);
        actualMin = glm::min(actualMin, worldCorner);
        actualMax = glm::max(actualMax, worldCorner);
    }

    outMin = actualMin;
    outMax = actualMax;
}

ofRectangle FileMesh::getScreenBoundingBox(ofCamera* cam)
{
    glm::vec3 min = const_cast<ofxAssimpModelLoader&>(model).getSceneMin();
    glm::vec3 max = const_cast<ofxAssimpModelLoader&>(model).getSceneMax();

    glm::vec3 corners[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {min.x, max.y, min.z}, {max.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {min.x, max.y, max.z}, {max.x, max.y, max.z}
    };

    float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
    for (auto& corner : corners)
    {
        glm::vec3 screenPos = cam->worldToScreen(corner + position);
        minX = std::min(minX, screenPos.x);
        maxX = std::max(maxX, screenPos.x);
        minY = std::min(minY, screenPos.y);
        maxY = std::max(maxY, screenPos.y);
    }
    return ofRectangle(minX, minY, maxX - minX, maxY - minY);
}

void FileMesh::drawBoundingBox()
{

}

FileMesh* FileMesh::copy() const
{
    return new FileMesh(*this);
}
