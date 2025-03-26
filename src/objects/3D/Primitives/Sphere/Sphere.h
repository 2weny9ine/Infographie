#pragma once
#include "Object3D.h"
#include "ofMesh.h"

class Sphere : public Object3D {
public:
    Sphere();
    Sphere(const Sphere& instance);
    ~Sphere() override;

    void setup() override;
    void draw() override;

    void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;

    void drawBoundingBox() override;

    Sphere* copy() const override;

private:
    ofSpherePrimitive sphere;
};
