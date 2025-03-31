#pragma once
#include "objects/Object3D.h"
#include "ofMesh.h"

class Cone : public Object3D {
public:
    Cone();
    Cone(const Cone& instance);
    ~Cone() override;

    void setup() override;
    void draw() override;

    void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;

    void drawBoundingBox() override;

    Cone* copy() const override;

private:
    ofMesh mesh;
    float radius;
    float height;
    int radialRes;
    void generateMesh();
};
