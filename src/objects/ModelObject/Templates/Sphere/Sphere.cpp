#include "Sphere.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>

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

Sphere::Sphere()
{
    ofLogNotice("Sphere::Sphere") << "Sphere constructor called.";
    setup();
}

Sphere::Sphere(const Sphere& other) : ModelObject(other), sphere(other.sphere)
{
    ofLogNotice("Sphere::Sphere(copy)") << "Sphere copy constructor called.";
    setup();
}

Sphere::~Sphere()
{
    ofLogNotice("Sphere::~Sphere") << "Sphere destructor called.";
}

void Sphere::setup()
{
    int resolution = 16;
    sphere.set(5.0f, resolution);
    sphere.setPosition(0, 0, 0);
}

void Sphere::draw()
{
    ofPushMatrix();

    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale);

    ofSetColor(color, opacity * 255);
    sphere.draw();

    if (selected)
    {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void Sphere::drawBoundingBox()
{
    ofPushStyle();
    ofSetColor(255);
    glLineWidth(2.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    sphere.drawWireframe();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0f);
    ofPopStyle();

    float radius = sphere.getRadius();
    glm::vec3 boxSize = glm::vec3(radius * 2.0f);
    glm::vec3 boxCenter = sphere.getPosition();
    glm::vec3 halfSize = boxSize * 0.5f;

    // Define the 8 vertices of the box.
    glm::vec3 v0 = boxCenter + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z);
    glm::vec3 v1 = boxCenter + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z);
    glm::vec3 v2 = boxCenter + glm::vec3(halfSize.x, halfSize.y, -halfSize.z);
    glm::vec3 v3 = boxCenter + glm::vec3(-halfSize.x, halfSize.y, -halfSize.z);
    glm::vec3 v4 = boxCenter + glm::vec3(-halfSize.x, -halfSize.y, halfSize.z);
    glm::vec3 v5 = boxCenter + glm::vec3(halfSize.x, -halfSize.y, halfSize.z);
    glm::vec3 v6 = boxCenter + glm::vec3(halfSize.x, halfSize.y, halfSize.z);
    glm::vec3 v7 = boxCenter + glm::vec3(-halfSize.x, halfSize.y, halfSize.z);

    // Fetch and convert the Bounding Box color.
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
            ofLogError("Sphere::drawBoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
        }
    }
    ofSetColor(boundingBoxColor);

    // Fetch the desired line thickness from configuration.
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
            ofLogError("Sphere::drawBoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("Sphere::drawBoundingBox") << "Invalid or empty line width format: '" << lineWidthStr << "'";
    }

    // --- Adjust thickness based on object's scale ---
    float avgScale = (std::abs(scale.x) + std::abs(scale.y) + std::abs(scale.z)) / 3.0f;
    boundingBoxThickness /= avgScale;
    // ---------------------------------------------------

    // Draw edges as thick lines.
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
}


Sphere* Sphere::copy() const
{
    return new Sphere(*this);
}
