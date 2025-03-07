#pragma once
#include "objects/ModelObject/ModelObject.h"
#include "ofMesh.h"

class Cone : public ModelObject {
public:
    Cone();
    Cone(const Cone& instance);
    ~Cone() override;

    void setup() override;
    void draw() override;
    void drawBoundingBox() override;
    Cone* copy() const override;

private:
    ofMesh mesh;
    float radius;
    float height;
    int radialRes;
    void generateMesh();
};
