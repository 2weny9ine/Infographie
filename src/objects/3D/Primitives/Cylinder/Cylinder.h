#pragma once

#include "objects/Object3D.h"
#include "ofMesh.h"

class Cylinder : public Object3D {
public:
    Cylinder();
    Cylinder(const Cylinder& instance);
    ~Cylinder() override;

    void setup() override;
    void draw() override;

    void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;

    void drawBoundingBox() override;

    Cylinder* copy() const override;

private:
    ofMesh mesh;
    float radius;
    float height;
    int radialRes;
    int heightRes;
    void generateMesh();
};
