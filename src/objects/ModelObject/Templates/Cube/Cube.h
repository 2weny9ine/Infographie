#pragma once
#include "objects/ModelObject/ModelObject.h"

class Cube : public ModelObject {
public:
    Cube();
    Cube(const Cube& instance);
    ~Cube() override;

    void setup() override;
    void draw() override;
    void drawBoundingBox() override;

    Cube* copy() const override;

private:
    ofBoxPrimitive cube;
};
