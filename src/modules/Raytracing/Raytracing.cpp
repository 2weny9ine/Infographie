#include "Raytracing.h"
#include <limits>

void Raytracing::addObject(Object3D* object)
{
    objects.push_back(object);
}

bool Raytracing::trace(const Ray& ray, Intersection& closestIsect) const
{
    Intersection tempIsect;
    bool hitAnything = false;
    float closestDist = std::numeric_limits<float>::infinity();

    for (auto obj : objects)
    {
        Intersection isect;
        if (obj->intersect(ray, isect) && isect.distance < closestDist)
        {
            hitAnything = true;
            closestDist = isect.distance;
            closestIsect = isect;
        }
    }

    return hitAnything;
}
