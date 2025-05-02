#include "PathTracer.h"
#include "enums/SurfaceType.h"
#include <glm/glm.hpp>
#include "ofLog.h"

PathTracer::PathTracer(Raytracing& rt)
    : tracer(rt)
    , maxDepth(Configuration::getInt("Raytracing.Max Depth"))
    , ambient(Configuration::getFloat("Raytracing.Ambient"))
    , iorGlass(Configuration::getFloat("Raytracing.IOR Glass"))
{
    ofLogNotice("PathTracer") << "init depth=" << maxDepth
        << " amb=" << ambient
        << " ior=" << iorGlass;
}

ofColor PathTracer::trace(const Ray& r, unsigned d) const
{
    return shade(r, d);
}

ofColor PathTracer::shade(const Ray& r, unsigned d) const
{
    Intersection I;
    if (!tracer.trace(r, I)) return ofColor(0);

    auto  base = I.object->getColor();
    float opac = glm::clamp(I.object->getOpacity(), 0.f, 1.f);

    ofColor col(
        static_cast<unsigned char>(base.r * ambient),
        static_cast<unsigned char>(base.g * ambient),
        static_cast<unsigned char>(base.b * ambient));

    if (d >= maxDepth) return col;

    switch (I.object->getSurfaceType())
    {
        case SurfaceType::MIRROR:
        {
            auto R = glm::reflect(r.direction, I.normal);
            return trace(Ray(I.point + R * 1e-4f, R), d + 1);
        }
        case SurfaceType::GLASS:
        {
            float eta = 1.f / iorGlass;
            auto  N = I.normal;
            float cosI = -glm::dot(r.direction, N);
            if (cosI < 0.f) { N = -N; cosI = -cosI; eta = iorGlass; }

            auto T = glm::refract(r.direction, N, eta);
            float R0 = pow((iorGlass - 1.f) / (iorGlass + 1.f), 2.f);
            float Fr = R0 + (1.f - R0) * pow(1.f - cosI, 5.f);

            if (glm::length2(T) < 1e-4f)
            {
                auto R = glm::reflect(r.direction, N);
                return trace(Ray(I.point + R * 1e-4f, R), d + 1);
            }

            auto R = glm::reflect(r.direction, N);
            auto rc = trace(Ray(I.point + R * 1e-4f, R), d + 1);
            auto tc = trace(Ray(I.point + T * 1e-4f, T), d + 1);

            col.r = static_cast<unsigned char>(rc.r * Fr + tc.r * (1.f - Fr));
            col.g = static_cast<unsigned char>(rc.g * Fr + tc.g * (1.f - Fr));
            col.b = static_cast<unsigned char>(rc.b * Fr + tc.b * (1.f - Fr));

            if (opac > 0.001f)
            {
                col.r = static_cast<unsigned char>(col.r * (1.f - opac) + base.r * opac);
                col.g = static_cast<unsigned char>(col.g * (1.f - opac) + base.g * opac);
                col.b = static_cast<unsigned char>(col.b * (1.f - opac) + base.b * opac);
            }
            return col;
        }
        default: break;
    }

    if (opac < 0.999f)
    {
        auto bg = trace(Ray(I.point + r.direction * 1e-3f, r.direction), d + 1);
        col.r = static_cast<unsigned char>(col.r * opac + bg.r * (1.f - opac));
        col.g = static_cast<unsigned char>(col.g * opac + bg.g * (1.f - opac));
        col.b = static_cast<unsigned char>(col.b * opac + bg.b * (1.f - opac));
    }
    return col;
}
