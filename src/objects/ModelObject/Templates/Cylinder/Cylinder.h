#pragma once
#include "objects/ModelObject/ModelObject.h"
#include "ofMesh.h"

class Cylinder : public ModelObject {
public:
    Cylinder();
    Cylinder(const Cylinder& instance);
    ~Cylinder() override;

    void setup() override;
    void draw() override;
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
