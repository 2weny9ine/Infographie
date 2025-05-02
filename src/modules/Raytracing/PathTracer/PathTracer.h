#pragma once
#include "modules/Configuration/Configuration.h"
#include "modules/Raytracing/Raytracing.h"
#include "ofColor.h"

class PathTracer {
public:
    explicit PathTracer(Raytracing& rt);

    ofColor trace(const Ray& r, unsigned depth = 0) const;

    void setMaxDepth(unsigned d) { maxDepth = d; }
    void setAmbient(float a) { ambient = a; }
    void setIOR(float v) { iorGlass = v; }

private:
    ofColor shade(const Ray& r, unsigned depth) const;

    Raytracing& tracer;
    unsigned    maxDepth;
    float       ambient;
    float       iorGlass;
};
