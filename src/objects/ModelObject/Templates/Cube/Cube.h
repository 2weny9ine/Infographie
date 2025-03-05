#pragma once
#include "objects/ModelObject/ModelObject.h"

class Cube : public ModelObject {
public:
    Cube();
    ~Cube() override;

    void setup() override;
    void draw() override;

    void drawBoundingBox() override;

private:
    ofBoxPrimitive cube;
};
