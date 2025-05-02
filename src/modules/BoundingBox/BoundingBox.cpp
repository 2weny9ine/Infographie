#include "BoundingBox.h"

void BoundingBox::computeOverallBounds(const std::vector<Object3D*>& objects)
{
    glm::vec3 tempMin(FLT_MAX), tempMax(-FLT_MAX);
    bool hasValid = false;

    for (auto* obj : objects)
    {
        if (!obj) continue;
        if (obj->getScale().x < 1.0f) continue;

        glm::vec3 objMin, objMax;
        obj->getWorldBounds(objMin, objMax);

        tempMin = glm::min(tempMin, objMin);
        tempMax = glm::max(tempMax, objMax);
        hasValid = true;
    }

    if (!hasValid)
    {
        minCorner = glm::vec3(0);
        maxCorner = glm::vec3(0);
        size      = glm::vec3(0);
        center    = glm::vec3(0);
    }
    else
    {
        minCorner = tempMin;
        maxCorner = tempMax;
        size      = maxCorner - minCorner;
        center    = (minCorner + maxCorner) * 0.5f;
    }
}



void BoundingBox::draw()
{
    if (glm::length(size) < 0.0001f)
        return;

    std::string lineColor = Configuration::get("Bounding Box.Line Color");
    ofColor boundingBoxColor = ofColor(255);
    if (!lineColor.empty())
    {
        if (lineColor[0] == '#')
            lineColor.erase(0, 1);
        boundingBoxColor = ofColor::fromHex(std::stoul(lineColor, nullptr, 16));
    }
    ofSetColor(boundingBoxColor);

    float boundingBoxThickness = 2.0f;
    std::string lineWidthStr = Configuration::get("Bounding Box.Line Width");
    if (!lineWidthStr.empty())
        boundingBoxThickness = std::stof(lineWidthStr);

    drawThickBoundingBox(center, size, boundingBoxThickness);
}

void BoundingBox::drawThickBoundingBox(const glm::vec3& center, const glm::vec3& size, float thickness)
{
    glm::vec3 halfSize = size * 0.5f;

    glm::vec3 corners[8] = {
        center + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),
        center + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),
        center + glm::vec3(halfSize.x,  halfSize.y, -halfSize.z),
        center + glm::vec3(-halfSize.x,  halfSize.y, -halfSize.z),
        center + glm::vec3(-halfSize.x, -halfSize.y,  halfSize.z),
        center + glm::vec3(halfSize.x, -halfSize.y,  halfSize.z),
        center + glm::vec3(halfSize.x,  halfSize.y,  halfSize.z),
        center + glm::vec3(-halfSize.x,  halfSize.y,  halfSize.z)
    };

    drawThickLine(corners[0], corners[1], thickness);
    drawThickLine(corners[1], corners[2], thickness);
    drawThickLine(corners[2], corners[3], thickness);
    drawThickLine(corners[3], corners[0], thickness);

    drawThickLine(corners[4], corners[5], thickness);
    drawThickLine(corners[5], corners[6], thickness);
    drawThickLine(corners[6], corners[7], thickness);
    drawThickLine(corners[7], corners[4], thickness);

    drawThickLine(corners[0], corners[4], thickness);
    drawThickLine(corners[1], corners[5], thickness);
    drawThickLine(corners[2], corners[6], thickness);
    drawThickLine(corners[3], corners[7], thickness);

    ofFill();
    float sphereRadius = thickness * 0.5f;
    for (auto& corner : corners)
        ofDrawSphere(corner, sphereRadius);
}

void BoundingBox::drawThickLine(const glm::vec3& start, const glm::vec3& end, float thickness)
{
    glm::vec3 edge = end - start;
    float length = glm::length(edge);
    if (length <= 0.0f) return;

    glm::vec3 midPoint = start + edge * 0.5f;
    glm::vec3 defaultDir(0, 0, 1);
    glm::quat rotation = glm::rotation(defaultDir, glm::normalize(edge));

    ofPushMatrix();
    ofTranslate(midPoint);
    ofMultMatrix(glm::mat4_cast(rotation));
    ofDrawBox(0, 0, 0, thickness, thickness, length);
    ofPopMatrix();
}
