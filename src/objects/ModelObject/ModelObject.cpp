#include "ModelObject.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>

// Helper function to draw a thick line (as a box) between two points.
static void drawThickLine(const glm::vec3& start, const glm::vec3& end, float thickness)
{
    glm::vec3 edge = end - start;
    float length = glm::length(edge);
    if (length <= 0.0f) return;

    glm::vec3 midPoint = start + edge * 0.5f;

    glm::vec3 defaultDir(0, 0, 1);
    glm::vec3 edgeDir = glm::normalize(edge);

    glm::quat rotation = glm::rotation(defaultDir, edgeDir);

    ofPushMatrix();
    ofTranslate(midPoint);
    ofMultMatrix(glm::mat4_cast(rotation));
    ofDrawBox(0, 0, 0, thickness, thickness, length);
    ofPopMatrix();
}

ModelObject::ModelObject() : modelPath("") {}

ModelObject::ModelObject(const ModelObject& other)
    : Object3D(other), modelPath(other.modelPath)
{
    loadModel(modelPath);
}

ModelObject::~ModelObject() {}

bool ModelObject::loadModel(const std::string& path)
{
    modelPath = path;
    return model.loadModel(path, true);
}

std::string ModelObject::getModelPath() const
{
    return modelPath;
}

void ModelObject::setup() {}

void ModelObject::update(float dt) {}

void ModelObject::draw()
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

void ModelObject::drawBoundingBox()
{
    if (model.getNumMeshes() == 0)
    {
        ofLogError("ModelObject::drawBoundingBox") << "No meshes loaded!";
        return;
    }

    // Get the scene bounds from the model and convert them to world coordinates.
    glm::vec3 sceneMin = model.getSceneMin();
    glm::vec3 sceneMax = model.getSceneMax();

    glm::mat4 modelMatrix = model.getModelMatrix();
    glm::vec3 worldMin = glm::vec3(modelMatrix * glm::vec4(sceneMin, 1.0));
    glm::vec3 worldMax = glm::vec3(modelMatrix * glm::vec4(sceneMax, 1.0));
    glm::vec3 boxSize = worldMax - worldMin;
    glm::vec3 boxCenter = (worldMin + worldMax) * 0.5f;
    glm::vec3 halfSize = boxSize * 0.5f;

    // Push matrix and translate to the center of the bounding box.
    ofPushMatrix();
    ofTranslate(boxCenter);

    // Fetch and convert Bounding Box color.
    std::string lineColor = Configuration::get("Bounding Box.Line Color");
    ofColor boundingBoxColor = ofColor(255);
    if (!lineColor.empty())
    {
        try
        {
            if (lineColor[0] == '#')
            {
                lineColor = lineColor.substr(1);
            }
            boundingBoxColor = ofColor::fromHex(std::stoul(lineColor, nullptr, 16));
        }
        catch (const std::exception& e)
        {
            ofLogError("ModelObject::drawBoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
        }
    }
    ofSetColor(boundingBoxColor);

    // Fetch the desired line thickness from configuration.
    std::string lineWidthStr = Configuration::get("Bounding Box.Line Width");
    float boundingBoxThickness = 2.0f;
    if (!lineWidthStr.empty() && std::all_of(lineWidthStr.begin(), lineWidthStr.end(), [](char c) { return std::isdigit(c) || c == '.'; }))
    {
        try
        {
            boundingBoxThickness = std::stof(lineWidthStr);
        }
        catch (const std::exception& e)
        {
            ofLogError("ModelObject::drawBoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("ModelObject::drawBoundingBox") << "Invalid or empty line width format: '" << lineWidthStr << "'";
    }

    // --- Adjustment for consistent thickness ---
    // Divide the thickness by the average absolute scale applied to the model.
    // (This assumes ModelObject has a member 'scale' of type glm::vec3.)
    float avgScale = (std::abs(scale.x) + std::abs(scale.y) + std::abs(scale.z)) / 3.0f;
    boundingBoxThickness /= avgScale;
    // --------------------------------------------

    // Compute the 8 vertices of the bounding box in local space (centered at (0,0,0)).
    glm::vec3 v0(-halfSize.x, -halfSize.y, -halfSize.z);
    glm::vec3 v1(halfSize.x, -halfSize.y, -halfSize.z);
    glm::vec3 v2(halfSize.x, halfSize.y, -halfSize.z);
    glm::vec3 v3(-halfSize.x, halfSize.y, -halfSize.z);
    glm::vec3 v4(-halfSize.x, -halfSize.y, halfSize.z);
    glm::vec3 v5(halfSize.x, -halfSize.y, halfSize.z);
    glm::vec3 v6(halfSize.x, halfSize.y, halfSize.z);
    glm::vec3 v7(-halfSize.x, halfSize.y, halfSize.z);

    // Draw edges as thick lines.
    // Bottom face
    drawThickLine(v0, v1, boundingBoxThickness);
    drawThickLine(v1, v2, boundingBoxThickness);
    drawThickLine(v2, v3, boundingBoxThickness);
    drawThickLine(v3, v0, boundingBoxThickness);
    // Top face
    drawThickLine(v4, v5, boundingBoxThickness);
    drawThickLine(v5, v6, boundingBoxThickness);
    drawThickLine(v6, v7, boundingBoxThickness);
    drawThickLine(v7, v4, boundingBoxThickness);
    // Vertical edges
    drawThickLine(v0, v4, boundingBoxThickness);
    drawThickLine(v1, v5, boundingBoxThickness);
    drawThickLine(v2, v6, boundingBoxThickness);
    drawThickLine(v3, v7, boundingBoxThickness);

    // Draw spheres at each vertex to smooth out the edges.
    ofFill();
    float sphereRadius = boundingBoxThickness * 0.5f;
    ofDrawSphere(v0, sphereRadius);
    ofDrawSphere(v1, sphereRadius);
    ofDrawSphere(v2, sphereRadius);
    ofDrawSphere(v3, sphereRadius);
    ofDrawSphere(v4, sphereRadius);
    ofDrawSphere(v5, sphereRadius);
    ofDrawSphere(v6, sphereRadius);
    ofDrawSphere(v7, sphereRadius);

    ofPopMatrix();
}

ofRectangle ModelObject::getScreenBoundingBox(ofCamera* cam)
{
    glm::vec3 min = model.getSceneMin();
    glm::vec3 max = model.getSceneMax();

    glm::vec3 corners[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {min.x, max.y, min.z}, {max.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {min.x, max.y, max.z}, {max.x, max.y, max.z}
    };

    float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;
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

ModelObject* ModelObject::copy() const
{
    return new ModelObject(*this);
}
