#pragma once
#include "objects/ModelObject/ModelObject.h"

class Sphere : public ModelObject {
public:
    Sphere();
    Sphere(const Sphere& instance);
    ~Sphere() override;

    void setup() override;
    void draw() override;
    void drawBoundingBox() override;

    Sphere* copy() const override;

private:
    ofSpherePrimitive sphere;
};