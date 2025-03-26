#include "Cone.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

Cone::Cone() : radius(5.0f), height(10.0f), radialRes(16)
{
    ofLogNotice("Cone::Cone") << "Cone constructor called.";
    setup();
}

Cone::Cone(const Cone& instance)
    : Object3D(instance),
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
    ofPopMatrix();
}

void Cone::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
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

void Cone::drawBoundingBox()
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

Cone* Cone::copy() const
{
    return new Cone(*this);
}
