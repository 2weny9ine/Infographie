#include "ExportScene.h"
#include <functional>          // <-- pour std::function
#include "Scene.h"
#include "modules/Configuration/Configuration.h"
#include "modules/Raytracing/Raytracing.h"
#include "objects/Object3D.h"
#include "enums/SurfaceType.h"
#include "ofMain.h"
#include <algorithm>

namespace
{
    constexpr float AMBIENT = 0.20f;
    constexpr float IOR_GLASS = 1.50f;
}

ExportScene::ExportScene(Scene& scn) : scene(scn) {}

void ExportScene::setExportTriggered(bool trig)
{
    exportTriggered = trig;
    if (trig)
    {
        captureCount = 0;
        startTime = ofGetElapsedTimef();
        lastCaptureTime = startTime;
        beforeExport();
    }
}
void ExportScene::setExportDuration(float s) { exportDuration = std::max(0.f, s); }

void ExportScene::exportFrames(const std::string& base,
                               const std::string& ext)
{
    if (!exportTriggered) return;

    float now = ofGetElapsedTimef();
    if (now - startTime >= exportDuration)
    {
        exportTriggered = false;
        afterExport();
        return;
    }
    if (captureCount == 0 || (now - lastCaptureTime) >= 1.0f)
    {
        bool allNone = std::all_of(scene.objects.begin(), scene.objects.end(),
                                   [](Object3D* o) { return o->getSurfaceType() == SurfaceType::NONE; });
        if (allNone)
        {
            std::string stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
            ofSaveScreen("output/" + base + "_" + ofToString(captureCount) + stamp + "." + ext);
        }
        else
        {
            renderRaytracedFrame(base, ext);
        }
        ++captureCount;
        lastCaptureTime = now;
    }
}

void ExportScene::beforeExport()
{
    if (Configuration::get("Export.Show Grid") == "false")
        scene.grid->setVisible(false);
    if (Configuration::get("Export.Show Node") == "false")
        scene.setNodeVisible(false);
}
void ExportScene::afterExport()
{
    scene.grid->setVisible(true);
    scene.setNodeVisible(true);
}

void ExportScene::renderRaytracedFrame(const std::string& base,
                                       const std::string& ext)
{
    int w = ofGetWidth(), h = ofGetHeight();
    ofPixels pix; pix.allocate(w, h, OF_PIXELS_RGB);

    Raytracing tracer;
    for (auto* o : scene.objects) tracer.addObject(o);

    auto* cam = scene.camera;
    float nz = cam->getNearClip(), fz = cam->getFarClip();

    /* =====================================================================
       Fonction récursive (profondeur max = 1) pour calculer la couleur
       ===================================================================== */
    std::function<ofColor(const Ray&, int)> sampleColor;
    sampleColor = [&](const Ray& r, int depth)->ofColor
    {
        Intersection is;
        if (!tracer.trace(r, is)) return ofColor(0);

        Object3D* obj = is.object;
        ofColor  base = obj->getColor();
        float    opac = glm::clamp(obj->getOpacity(), 0.f, 1.f);

        // teinte ambiante
        ofColor amb(static_cast<unsigned char>(base.r * AMBIENT),
                    static_cast<unsigned char>(base.g * AMBIENT),
                    static_cast<unsigned char>(base.b * AMBIENT));

        ofColor front = amb;

        if (depth == 0)   // un seul rebond
        {
            switch (obj->getSurfaceType())
            {
                case SurfaceType::MIRROR:
                {
                    glm::vec3 R = glm::reflect(r.direction, is.normal);
                    front = sampleColor(Ray(is.point + R * 1e-4f, R), 1);
                    break;
                }
                case SurfaceType::GLASS:
                {
                    glm::vec3 T = glm::refract(r.direction, is.normal, 1.f / IOR_GLASS);
                    if (glm::length2(T) == 0) // réflexion interne
                        T = glm::reflect(r.direction, is.normal);
                    front = sampleColor(Ray(is.point + T * 1e-4f, T), 1);
                    // teinte verre
                    front.r = (unsigned char)(front.r * (1 - opac) + base.r * opac);
                    front.g = (unsigned char)(front.g * (1 - opac) + base.g * opac);
                    front.b = (unsigned char)(front.b * (1 - opac) + base.b * opac);
                    return front;
                }
                default: break;
            }
        }

        /* transparence : mix avec l’arrière-plan */
        if (opac < 0.999f && depth == 0)
        {
            Ray  behind(is.point + r.direction * 1e-3f, r.direction);
            ofColor bg = sampleColor(behind, 1);
            front.r = (unsigned char)(front.r * opac + bg.r * (1 - opac));
            front.g = (unsigned char)(front.g * opac + bg.g * (1 - opac));
            front.b = (unsigned char)(front.b * opac + bg.b * (1 - opac));
        }
        return front;
    };
    /* =================================================================== */

    for (int y = 0;y < h;++y)
    {
        for (int x = 0;x < w;++x)
        {
            glm::vec3 p0 = cam->screenToWorld({ (float)x,(float)y,nz });
            glm::vec3 p1 = cam->screenToWorld({ (float)x,(float)y,fz });
            glm::vec3 d = glm::normalize(p1 - p0);
            ofColor c = sampleColor(Ray(p0, d), 0);
            pix.setColor(x, y, c);
        }
    }

    std::string stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
    ofSaveImage(pix, "output/" + base + "_" + ofToString(captureCount) + stamp + "." + ext);
}
