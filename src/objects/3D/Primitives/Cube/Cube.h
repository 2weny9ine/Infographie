#pragma once

#include "objects/Object3D.h"
#include <glm/vec3.hpp>

class Cube : public Object3D {
public:
    Cube();
    Cube(const Cube& instance);
    ~Cube() override;

    void setup() override;
    void draw() override;

    void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;

    void drawBoundingBox() override;

    Cube* copy() const override;

private:
    float width;
    float height;
    float depth;
};
