#include "Cube.h"

Cube::Cube()
{
    ofLogNotice("Cube::Cube") << "Cube constructor called.";
    setup();
}

Cube::Cube(const Cube& instance) : ModelObject(instance), cube(instance.cube)
{
    ofLogNotice("Cube::Cube(copy)") << "Cube copy constructor called.";
    setup();
}

Cube::~Cube()
{
    ofLogNotice("Cube::~Cube") << "Cube destructor called.";
}

void Cube::setup()
{
    cube.set(10.0f, 10.0f, 10.0f);
    cube.setPosition(0, 0, 0);
}

void Cube::draw()
{
    ofPushMatrix();

    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale);

    ofSetColor(color, opacity * 255);
    cube.draw();

    if (selected)
    {
        drawBoundingBox();
    }

    ofPopMatrix();
}

void Cube::drawBoundingBox()
{
    ofSetColor(255);
    glLineWidth(2.0);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    cube.drawWireframe();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glLineWidth(1.0);

    glm::vec3 boxSize(1.0f, 1.0f, 1.0f);
    glm::vec3 boxCenter = cube.getPosition();

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
            ofLogError("Cube::drawBoundingBox") << "Invalid hex color format: " << lineColor << " - " << e.what();
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
            ofLogError("Cube::drawBoundingBox") << "Error converting line width: " << lineWidthStr << " - " << e.what();
        }
    }
    else
    {
        ofLogError("Cube::drawBoundingBox") << "Invalid or empty line width format: '" << lineWidthStr << "'";
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

Cube* Cube::copy() const
{
    return new Cube(*this);
}
