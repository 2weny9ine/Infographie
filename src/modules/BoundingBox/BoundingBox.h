#pragma once

#include "ofMain.h"
#include "modules/Configuration/Configuration.h"
#include "objects/Object3D.h"
#include <vector>
#include <algorithm>
#include <glm/gtx/quaternion.hpp>

class BoundingBox {
public:
    BoundingBox() = default;

    BoundingBox(const std::vector<Object3D*>& objects)
    {
        computeOverallBounds(objects);
    }

    void update(const std::vector<Object3D*>& objects)
    {
        computeOverallBounds(objects);
    }

    void draw();

private:
    glm::vec3 minCorner = glm::vec3(0);
    glm::vec3 maxCorner = glm::vec3(0);

    void computeOverallBounds(const std::vector<Object3D*>& objects);

    void drawThickBoundingBox(const glm::vec3& minPt,
                              const glm::vec3& maxPt,
                              float thickness);

    void drawThickLine(const glm::vec3& start,
                       const glm::vec3& end,
                       float thickness);
};
