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

std::vector<Property> Cylinder::getProperties() const
{
    std::vector<Property> props = Object3D::getProperties();

    std::vector<Property> cylinderProps = {
        {"surface type", PropertyType::SurfaceType, PropertyValue{static_cast<int>(surfaceType)} }
    };


    props.insert(props.end(), cylinderProps.begin(), cylinderProps.end());
    return props;
}

bool Cylinder::intersect(const Ray& ray, Intersection& intersection)
{
    // Convert ofVec3f rotation to glm::vec3 before calling glm::radians()
    glm::vec3 euler{ rotation.x, rotation.y, rotation.z };
    glm::quat q = glm::quat(glm::radians(euler));
    glm::quat invQ = glm::inverse(q);

    glm::vec3 localOrigin = invQ * ((ray.origin - position) / scale);
    glm::vec3 localDir = glm::normalize(invQ * (ray.direction / scale));

    float halfH = height * 0.5f;

    // Solve quadratic for cylindrical side
    float a = localDir.x * localDir.x + localDir.z * localDir.z;
    float b = 2.0f * (localOrigin.x * localDir.x + localOrigin.z * localDir.z);
    float c = localOrigin.x * localOrigin.x + localOrigin.z * localOrigin.z - radius * radius;
    float disc = b * b - 4 * a * c;

    float tSide = std::numeric_limits<float>::infinity();
    glm::vec3 nSide(0.0f);

    if (disc >= 0.0f && fabs(a) > 1e-6f)
    {
        float sq = sqrt(disc);
        float t0 = (-b - sq) / (2 * a);
        float t1 = (-b + sq) / (2 * a);
        for (float t : {t0, t1})
        {
            if (t > 0)
            {
                float y = localOrigin.y + t * localDir.y;
                if (y >= -halfH && y <= halfH && t < tSide)
                {
                    tSide = t;
                    glm::vec3 hit = localOrigin + localDir * t;
                    nSide = glm::normalize(glm::vec3(hit.x, 0, hit.z));
                }
            }
        }
    }

    // Caps
    float tCap = std::numeric_limits<float>::infinity();
    glm::vec3 nCap(0.0f);
    if (fabs(localDir.y) > 1e-6f)
    {
        for (float yc : { halfH, -halfH })
        {
            float t = (yc - localOrigin.y) / localDir.y;
            if (t > 0)
            {
                glm::vec3 hit = localOrigin + localDir * t;
                if (hit.x * hit.x + hit.z * hit.z <= radius * radius && t < tCap)
                {
                    tCap = t;
                    nCap = glm::vec3(0, yc > 0 ? 1 : -1, 0);
                }
            }
        }
    }

    float t = std::min(tSide, tCap);
    if (!std::isfinite(t)) return false;

    intersection.hit = true;
    intersection.distance = t;

    glm::vec3 localHit = localOrigin + localDir * t;
    glm::vec3 localN = (tSide < tCap) ? nSide : nCap;

    intersection.point = position + (q * (localHit * scale));
    intersection.normal = glm::normalize(q * localN);
    intersection.object = this;
    return true;
}