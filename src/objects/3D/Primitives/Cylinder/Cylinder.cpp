#include "Cylinder.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

Cylinder::Cylinder()
    : radius(5.0f), height(10.0f), radialRes(16), heightRes(1)
{
    ofLogNotice("Cylinder::Cylinder") << "Cylinder constructor called.";
    setup();
}

Cylinder::Cylinder(const Cylinder& instance)
    : Object3D(instance),
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
    ofPopMatrix();
}

void Cylinder::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
{
    glm::vec3 localMin(-radius, -height * 0.5f, -radius);
    glm::vec3 localMax(radius, height * 0.5f, radius);

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

void Cylinder::drawBoundingBox()
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

Cylinder* Cylinder::copy() const
{
    return new Cylinder(*this);
}
