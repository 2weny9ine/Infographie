#include "Sphere.h"

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
    ofSetColor(255);
    glLineWidth(2.0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    sphere.drawWireframe();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0);

    float radius = sphere.getRadius();
    glm::vec3 boxSize = glm::vec3(radius * 2.0f);
    glm::vec3 boxCenter = sphere.getPosition();

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
            ofLogError("Sphere::drawBoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
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
            ofLogError("Sphere::drawBoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("Sphere::drawBoundingBox") << "Invalid or empty line width format: '" << lineWidthStr << "'";
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

Sphere* Sphere::copy() const
{
    return new Sphere(*this);
}
