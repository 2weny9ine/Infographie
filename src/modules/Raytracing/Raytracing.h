#pragma once

#include <vector>
#include "modules/Raytracing/Ray/Ray.h"
#include "modules/Raytracing/Intersection/Intersection.h"
#include "Object3D.h"

class Raytracing {
public:
    Raytracing() = default;
    ~Raytracing() = default;

    void addObject(Object3D* object);

    bool trace(const Ray& ray, Intersection& closestIsect) const;

private:
    std::vector<Object3D*> objects;
};
