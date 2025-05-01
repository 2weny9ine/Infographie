#pragma once

#include <glm/vec3.hpp>
class Object3D;

struct Intersection {
    bool        hit;
    float       distance;
    glm::vec3   point;
    glm::vec3   normal;
    Object3D* object;

    Intersection();
};
