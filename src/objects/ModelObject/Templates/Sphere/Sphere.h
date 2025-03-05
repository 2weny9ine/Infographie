#pragma once
#include "objects/ModelObject/ModelObject.h"

class Sphere : public ModelObject {
public:
    Sphere();
    ~Sphere() override;

    void setup() override;
    void draw() override;


    void drawBoundingBox() override;

private:
    ofSpherePrimitive sphere;
};
