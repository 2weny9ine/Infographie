#include "BoundingBox.h"

void BoundingBox::draw()
{
    glm::vec3 boxSize = maxCorner - minCorner;
    if (glm::length(boxSize) < 0.0001f)
    {
        return;
    }

    std::string lineColor = Configuration::get("Bounding Box.Line Color");
    ofColor boundingBoxColor = ofColor(255);
    if (!lineColor.empty())
    {
        try
        {
            if (lineColor[0] == '#')
            {
                lineColor.erase(0, 1);
            }
            boundingBoxColor = ofColor::fromHex(std::stoul(lineColor, nullptr, 16));
        }
        catch (const std::exception& e)
        {
            ofLogError("BoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
        }
    }
    ofSetColor(boundingBoxColor);

    std::string lineWidthStr = Configuration::get("Bounding Box.Line Width");
    float boundingBoxThickness = 2.0f;
    if (!lineWidthStr.empty() &&
        std::all_of(lineWidthStr.begin(), lineWidthStr.end(),
                    [](char c) { return std::isdigit(c) || c == '.'; }))
    {
        try
        {
            boundingBoxThickness = std::stof(lineWidthStr);
        }
        catch (const std::exception& e)
        {
            ofLogError("BoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("BoundingBox") << "Invalid or empty line width: '" << lineWidthStr << "'";
    }

    drawThickBoundingBox(minCorner, maxCorner, boundingBoxThickness);
}

void BoundingBox::computeOverallBounds(const std::vector<Object3D*>& objects)
{
    bool first = true;
    for (auto* obj : objects)
    {
        if (!obj) continue;

        glm::vec3 objMin, objMax;
        obj->getWorldBounds(objMin, objMax);

        if (first)
        {
            minCorner = objMin;
            maxCorner = objMax;
            first = false;
        }
        else
        {
            minCorner.x = std::min(minCorner.x, objMin.x);
            minCorner.y = std::min(minCorner.y, objMin.y);
            minCorner.z = std::min(minCorner.z, objMin.z);
            maxCorner.x = std::max(maxCorner.x, objMax.x);
            maxCorner.y = std::max(maxCorner.y, objMax.y);
            maxCorner.z = std::max(maxCorner.z, objMax.z);
        }
    }

    if (first)
    {
        minCorner = glm::vec3(0);
        maxCorner = glm::vec3(0);
    }
}

void BoundingBox::drawThickBoundingBox(const glm::vec3& minPt,
                                       const glm::vec3& maxPt,
                                       float thickness)
{
    // Compute the 8 corners of the bounding box
    glm::vec3 v0(minPt.x, minPt.y, minPt.z);
    glm::vec3 v1(maxPt.x, minPt.y, minPt.z);
    glm::vec3 v2(maxPt.x, maxPt.y, minPt.z);
    glm::vec3 v3(minPt.x, maxPt.y, minPt.z);
    glm::vec3 v4(minPt.x, minPt.y, maxPt.z);
    glm::vec3 v5(maxPt.x, minPt.y, maxPt.z);
    glm::vec3 v6(maxPt.x, maxPt.y, maxPt.z);
    glm::vec3 v7(minPt.x, maxPt.y, maxPt.z);

    // Draw edges between corners
    drawThickLine(v0, v1, thickness);
    drawThickLine(v1, v2, thickness);
    drawThickLine(v2, v3, thickness);
    drawThickLine(v3, v0, thickness);

    drawThickLine(v4, v5, thickness);
    drawThickLine(v5, v6, thickness);
    drawThickLine(v6, v7, thickness);
    drawThickLine(v7, v4, thickness);

    drawThickLine(v0, v4, thickness);
    drawThickLine(v1, v5, thickness);
    drawThickLine(v2, v6, thickness);
    drawThickLine(v3, v7, thickness);

    // Draw spheres at each corner
    ofFill();
    float sphereRadius = thickness * 0.5f;
    ofDrawSphere(v0, sphereRadius);
    ofDrawSphere(v1, sphereRadius);
    ofDrawSphere(v2, sphereRadius);
    ofDrawSphere(v3, sphereRadius);
    ofDrawSphere(v4, sphereRadius);
    ofDrawSphere(v5, sphereRadius);
    ofDrawSphere(v6, sphereRadius);
    ofDrawSphere(v7, sphereRadius);
}

void BoundingBox::drawThickLine(const glm::vec3& start,
                                const glm::vec3& end,
                                float thickness)
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
