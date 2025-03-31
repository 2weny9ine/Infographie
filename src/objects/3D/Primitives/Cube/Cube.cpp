#include "Cube.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>

Cube::Cube()
{
    ofLogNotice("Cube::Cube") << "Cube constructor called.";
    setup();
}

Cube::Cube(const Cube& instance)
    : Object3D(instance)
    , width(instance.width)
    , height(instance.height)
    , depth(instance.depth)
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
    width = 10.0f;
    height = 10.0f;
    depth = 10.0f;
}

void Cube::draw()
{
    ofPushMatrix();

    // Apply local transformations (position, rotation, scale)
    ofTranslate(position);
    ofRotateXDeg(rotation.x);
    ofRotateYDeg(rotation.y);
    ofRotateZDeg(rotation.z);
    ofScale(scale.x, scale.y, scale.z);

    ofSetColor(color, opacity * 255);

    // Draw a box centered at (0,0,0)
    ofDrawBox(0, 0, 0, width, height, depth);

    ofPopMatrix();
}

void Cube::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
{
    float w2 = width * 0.5f;
    float h2 = height * 0.5f;
    float d2 = depth * 0.5f;
    glm::vec3 localMin(-w2, -h2, -d2);
    glm::vec3 localMax(w2, h2, d2);

    ofMatrix4x4 RS;
    RS.makeIdentityMatrix();
    RS.rotate(rotation.x, 1, 0, 0);
    RS.rotate(rotation.y, 0, 1, 0);
    RS.rotate(rotation.z, 0, 0, 1);
    RS.scale(scale.x, scale.y, scale.z);

    glm::vec3 corners[8] = {
        { localMin.x, localMin.y, localMin.z },
        { localMax.x, localMin.y, localMin.z },
        { localMin.x, localMax.y, localMin.z },
        { localMax.x, localMax.y, localMin.z },
        { localMin.x, localMin.y, localMax.z },
        { localMax.x, localMin.y, localMax.z },
        { localMin.x, localMax.y, localMax.z },
        { localMax.x, localMax.y, localMax.z }
    };

    glm::vec3 actualMin(999999.f);
    glm::vec3 actualMax(-999999.f);

    const float* m = RS.getPtr();
    for (auto& c : corners)
    {
        float x = c.x * m[0] + c.y * m[4] + c.z * m[8];
        float y = c.x * m[1] + c.y * m[5] + c.z * m[9];
        float z = c.x * m[2] + c.y * m[6] + c.z * m[10];
        glm::vec3 transformed(x, y, z);

        glm::vec3 worldCorner = transformed + position;
        actualMin = glm::min(actualMin, worldCorner);
        actualMax = glm::max(actualMax, worldCorner);
    }

    outMin = actualMin;
    outMax = actualMax;
}

void Cube::drawBoundingBox()
{
    glm::vec3 bbMin, bbMax;
    getWorldBounds(bbMin, bbMax);
    glm::vec3 center = (bbMin + bbMax) * 0.5f;
    float w = bbMax.x - bbMin.x;
    float h = bbMax.y - bbMin.y;
    float d = bbMax.z - bbMin.z;
    ofPushStyle();
    ofNoFill();
    ofSetColor(strokeColor);
    ofSetLineWidth(lineWidth);
    ofDrawBox(center, w, h, d);
    ofPopStyle();
}

Cube* Cube::copy() const
{
    return new Cube(*this);
}
