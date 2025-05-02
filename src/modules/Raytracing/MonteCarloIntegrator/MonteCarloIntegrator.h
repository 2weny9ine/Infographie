#pragma once
#include "modules/Raytracing/PathTracer/PathTracer.h"
#include "ofPixels.h"
#include "ofCamera.h"

class MonteCarloIntegrator {
public:
    MonteCarloIntegrator(PathTracer& pt, ofCamera* c,
                         int w, int h, int spp);

    void render(ofPixels& out);   // multithread si OpenMP dispo

private:
    PathTracer& tracer;
    ofCamera* cam;
    int         W, H, S;
};
