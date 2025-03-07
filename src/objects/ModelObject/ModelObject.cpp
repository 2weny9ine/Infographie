#include "ModelObject.h"

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

    // Fetch and convert Bounding Box color
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

    std::string lineWidthStr = Configuration::get("Bounding Box.Line Width");
    float boundingBoxWidth = 2.0f;

    if (!lineWidthStr.empty() && std::all_of(lineWidthStr.begin(), lineWidthStr.end(), [](char c) { return std::isdigit(c) || c == '.'; }))
    {
        try
        {
            boundingBoxWidth = std::stof(lineWidthStr);
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

    ofSetLineWidth(boundingBoxWidth);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    ofDrawBox(glm::vec3(0, 0, 0), boxSize.x, boxSize.y, boxSize.z);

    glDisable(GL_CULL_FACE);
    ofFill();
    ofSetLineWidth(1.0f);

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
