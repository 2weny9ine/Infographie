#include "Cone.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

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

Cone::Cone() : radius(5.0f), height(10.0f), radialRes(16)
{
    ofLogNotice("Cone::Cone") << "Cone constructor called.";
    setup();
}

Cone::Cone(const Cone& instance)
    : ModelObject(instance),
    mesh(instance.mesh),
    radius(instance.radius),
    height(instance.height),
    radialRes(instance.radialRes)
{
    ofLogNotice("Cone::Cone(copy)") << "Cone copy constructor called.";
    setup();
}

Cone::~Cone()
{
    ofLogNotice("Cone::~Cone") << "Cone destructor called.";
}

void Cone::setup()
{
    radius = 5.0f;
    height = 10.0f;
    radialRes = 16;
    generateMesh();
}

void Cone::generateMesh()
{
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    float slant = std::sqrt(radius * radius + height * height);
    for (int i = 0; i < radialRes; i++)
    {
        float theta = (TWO_PI * i) / radialRes;
        float thetaNext = (TWO_PI * (i + 1)) / radialRes;
        glm::vec3 n1 = glm::normalize(glm::vec3(cos(theta), radius / slant, sin(theta)));
        glm::vec3 n2 = glm::normalize(glm::vec3(cos(thetaNext), radius / slant, sin(thetaNext)));
        glm::vec3 apexNormal = glm::normalize(n1 + n2);
        int apexIndex = mesh.getNumVertices();
        mesh.addVertex(glm::vec3(0, height * 0.5f, 0));
        mesh.addNormal(apexNormal);
        int v1Index = mesh.getNumVertices();
        mesh.addVertex(glm::vec3(radius * cos(theta), -height * 0.5f, radius * sin(theta)));
        mesh.addNormal(n1);
        int v2Index = mesh.getNumVertices();
        mesh.addVertex(glm::vec3(radius * cos(thetaNext), -height * 0.5f, radius * sin(thetaNext)));
        mesh.addNormal(n2);
        mesh.addIndex(apexIndex);
        mesh.addIndex(v1Index);
        mesh.addIndex(v2Index);
    }
    int baseCenterIndex = mesh.getNumVertices();
    mesh.addVertex(glm::vec3(0, -height * 0.5f, 0));
    mesh.addNormal(glm::vec3(0, -1, 0));
    for (int i = 0; i < radialRes; i++)
    {
        float theta = (TWO_PI * i) / radialRes;
        int idx = mesh.getNumVertices();
        mesh.addVertex(glm::vec3(radius * cos(theta), -height * 0.5f, radius * sin(theta)));
        mesh.addNormal(glm::vec3(0, -1, 0));
    }
    for (int i = 0; i < radialRes; i++)
    {
        int current = baseCenterIndex + 1 + i;
        int next = baseCenterIndex + 1 + ((i + 1) % radialRes);
        mesh.addIndex(baseCenterIndex);
        mesh.addIndex(next);
        mesh.addIndex(current);
    }
}

void Cone::draw()
{
    ofPushMatrix();
    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale);
    ofSetColor(color, opacity * 255);
    mesh.draw();
    if (selected)
    {
        drawBoundingBox();
    }
    ofPopMatrix();
}

void Cone::drawBoundingBox()
{
    ofPushStyle();
    ofSetColor(255);
    glLineWidth(2.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    mesh.drawWireframe();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0f);
    ofPopStyle();
    glm::vec3 boxCenter = position;
    float boxWidth = radius * 2.0f;
    float boxHeight = height;
    glm::vec3 halfSize(boxWidth * 0.5f, boxHeight * 0.5f, boxWidth * 0.5f);
    glm::vec3 v0 = boxCenter + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z);
    glm::vec3 v1 = boxCenter + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z);
    glm::vec3 v2 = boxCenter + glm::vec3(halfSize.x, halfSize.y, -halfSize.z);
    glm::vec3 v3 = boxCenter + glm::vec3(-halfSize.x, halfSize.y, -halfSize.z);
    glm::vec3 v4 = boxCenter + glm::vec3(-halfSize.x, -halfSize.y, halfSize.z);
    glm::vec3 v5 = boxCenter + glm::vec3(halfSize.x, -halfSize.y, halfSize.z);
    glm::vec3 v6 = boxCenter + glm::vec3(halfSize.x, halfSize.y, halfSize.z);
    glm::vec3 v7 = boxCenter + glm::vec3(-halfSize.x, halfSize.y, halfSize.z);
    std::string lineColor = Configuration::get("Bounding Box.Line Color");
    ofColor boundingBoxColor = ofColor(255);
    if (!lineColor.empty())
    {
        try
        {
            if (lineColor[0] == '#')
                lineColor = lineColor.substr(1);
            boundingBoxColor = ofColor::fromHex(std::stoul(lineColor, nullptr, 16));
        }
        catch (const std::exception& e)
        {
            ofLogError("Cone::drawBoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
        }
    }
    ofSetColor(boundingBoxColor);
    std::string lineWidthStr = Configuration::get("Bounding Box.Line Width");
    float boundingBoxThickness = 2.0f;
    if (!lineWidthStr.empty() &&
        std::all_of(lineWidthStr.begin(), lineWidthStr.end(), [](char c) { return std::isdigit(c) || c == '.'; }))
    {
        try
        {
            boundingBoxThickness = std::stof(lineWidthStr);
        }
        catch (const std::exception& e)
        {
            ofLogError("Cone::drawBoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("Cone::drawBoundingBox") << "Invalid or empty line width format: '" << lineWidthStr << "'";
    }
    float avgScale = (std::abs(scale.x) + std::abs(scale.y) + std::abs(scale.z)) / 3.0f;
    boundingBoxThickness /= avgScale;
    drawThickLine(v0, v1, boundingBoxThickness);
    drawThickLine(v1, v2, boundingBoxThickness);
    drawThickLine(v2, v3, boundingBoxThickness);
    drawThickLine(v3, v0, boundingBoxThickness);
    drawThickLine(v4, v5, boundingBoxThickness);
    drawThickLine(v5, v6, boundingBoxThickness);
    drawThickLine(v6, v7, boundingBoxThickness);
    drawThickLine(v7, v4, boundingBoxThickness);
    drawThickLine(v0, v4, boundingBoxThickness);
    drawThickLine(v1, v5, boundingBoxThickness);
    drawThickLine(v2, v6, boundingBoxThickness);
    drawThickLine(v3, v7, boundingBoxThickness);
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
}

Cone* Cone::copy() const
{
    return new Cone(*this);
}
