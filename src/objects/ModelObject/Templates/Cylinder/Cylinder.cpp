#include "Cylinder.h"
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

Cylinder::Cylinder()
    : radius(5.0f), height(10.0f), radialRes(16), heightRes(1)
{
    ofLogNotice("Cylinder::Cylinder") << "Cylinder constructor called.";
    setup();
}

Cylinder::Cylinder(const Cylinder& instance)
    : ModelObject(instance),
    mesh(instance.mesh),
    radius(instance.radius),
    height(instance.height),
    radialRes(instance.radialRes),
    heightRes(instance.heightRes)
{
    ofLogNotice("Cylinder::Cylinder(copy)") << "Cylinder copy constructor called.";
    setup();
}

Cylinder::~Cylinder()
{
    ofLogNotice("Cylinder::~Cylinder") << "Cylinder destructor called.";
}

void Cylinder::setup()
{
    radius = 5.0f;
    height = 10.0f;
    radialRes = 16;
    heightRes = 1;
    generateMesh();
}

void Cylinder::generateMesh()
{
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for (int i = 0; i <= radialRes; i++)
    {
        float theta = (TWO_PI * i) / radialRes;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        mesh.addVertex(glm::vec3(x, -height * 0.5f, z));
        mesh.addVertex(glm::vec3(x, height * 0.5f, z));
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));
        mesh.addNormal(normal);
        mesh.addNormal(normal);
    }
    for (int i = 0; i < radialRes; i++)
    {
        int index = i * 2;
        mesh.addIndex(index);
        mesh.addIndex(index + 1);
        mesh.addIndex(index + 2);
        mesh.addIndex(index + 1);
        mesh.addIndex(index + 3);
        mesh.addIndex(index + 2);
    }
    int topCenterIndex = mesh.getNumVertices();
    mesh.addVertex(glm::vec3(0, height * 0.5f, 0));
    mesh.addNormal(glm::vec3(0, 1, 0));
    int bottomCenterIndex = mesh.getNumVertices();
    mesh.addVertex(glm::vec3(0, -height * 0.5f, 0));
    mesh.addNormal(glm::vec3(0, -1, 0));
    for (int i = 0; i < radialRes; i++)
    {
        int index = i * 2 + 1;
        int nextIndex = ((i + 1) % radialRes) * 2 + 1;
        mesh.addIndex(topCenterIndex);
        mesh.addIndex(index);
        mesh.addIndex(nextIndex);
    }
    for (int i = 0; i < radialRes; i++)
    {
        int index = i * 2;
        int nextIndex = ((i + 1) % radialRes) * 2;
        mesh.addIndex(bottomCenterIndex);
        mesh.addIndex(nextIndex);
        mesh.addIndex(index);
    }
}

void Cylinder::draw()
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

void Cylinder::drawBoundingBox()
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
    float width = radius * 2.0f;
    float depth = radius * 2.0f;
    float halfHeight = height * 0.5f;
    glm::vec3 halfSize(width * 0.5f, halfHeight, depth * 0.5f);
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
            ofLogError("Cylinder::drawBoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
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
            ofLogError("Cylinder::drawBoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("Cylinder::drawBoundingBox") << "Invalid or empty line width format: '" << lineWidthStr << "'";
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



Cylinder* Cylinder::copy() const
{
    return new Cylinder(*this);
}
