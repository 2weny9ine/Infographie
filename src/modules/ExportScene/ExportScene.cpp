#include "ExportScene.h"
#include "Scene.h"
#include "modules/Configuration/Configuration.h"
#include "modules/Raytracing/Raytracing.h"
#include "modules/Raytracing/PathTracer/PathTracer.h"
#include "modules/Raytracing/MonteCarloIntegrator/MonteCarloIntegrator.h"
#include "objects/Object3D.h"
#include "enums/SurfaceType.h"
#include "ofMain.h"
#include "ofLog.h"
#include <algorithm>

static int   cfgInt(const std::string& k, int   d)
{
    try { return Configuration::getInt(k); }
    catch (...) { return d; }
}
static float cfgFloat(const std::string& k, float d)
{
    try { return Configuration::getFloat(k); }
    catch (...) { return d; }
}

ExportScene::ExportScene(Scene& scn)
    : scene(scn)
    , exportTriggered(false)
    , captureCount(0)
    , startTime(0.f)
    , lastCaptureTime(0.f)
    , exportDuration(0.f)
    , lastPercent(-1)
{}

void ExportScene::setExportTriggered(bool trig)
{
    exportTriggered = trig;
    if (trig)
    {
        originalTitle = "Infographie";
        captureCount = 0;
        startTime = ofGetElapsedTimef();
        lastCaptureTime = startTime;
        lastPercent = -1;
        beforeExport();
        updateWindowTitle(startTime);
        ofLogNotice("ExportScene") << "export started";
    }
    else
    {
        afterExport();
    }
}

void ExportScene::setExportDuration(float s)
{
    exportDuration = std::max(0.f, s);
}

void ExportScene::exportFrames(const std::string& base,
                               const std::string& ext)
{
    if (!exportTriggered) return;

    float now = ofGetElapsedTimef();
    updateWindowTitle(now);

    if (exportDuration > 0.f && now - startTime >= exportDuration)
    {
        exportTriggered = false;
        afterExport();
        ofLogNotice("ExportScene") << "export finished";
        return;
    }

    if (captureCount == 0 || now - lastCaptureTime >= 1.f)
    {
        bool none = std::all_of(scene.objects.begin(), scene.objects.end(),
                                [](Object3D* o) { return o->getSurfaceType() == SurfaceType::NONE; });

        if (none)
        {
            std::string stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
            ofSaveScreen("output/" + base + "_" +
                         ofToString(captureCount) + stamp + "." + ext);
            ofLogNotice("ExportScene") << "saved raw screen";
        }
        else
        {
            ofLogNotice("ExportScene") << "raytrace frame " << captureCount;
            renderRaytracedFrame(base, ext);
        }
        ++captureCount;
        lastCaptureTime = now;
    }
}

void ExportScene::beforeExport()
{
    if (Configuration::get("Export.Show Grid") == "false" && scene.grid)
        scene.grid->setVisible(false);
    if (Configuration::get("Export.Show Node") == "false")
        scene.setNodeVisible(false);
}

void ExportScene::afterExport()
{
    if (scene.grid) scene.grid->setVisible(true);
    scene.setNodeVisible(true);
    ofSetWindowTitle(originalTitle.empty() ? "Infographie" : originalTitle);
}

void ExportScene::renderRaytracedFrame(const std::string& base,
                                       const std::string& ext)
{
    int w = ofGetWidth(), h = ofGetHeight();

    Raytracing rt;
    for (auto* o : scene.objects) rt.addObject(o);

    PathTracer pt(rt);
    pt.setMaxDepth(cfgInt("Raytracing.Max Depth", 5));
    pt.setAmbient(cfgFloat("Raytracing.Ambient", 1.f));
    pt.setIOR(cfgFloat("Raytracing.IOR Glass", 1.5f));

    int spp = cfgInt("Raytracing.Samples Per Pixel", 16);
    MonteCarloIntegrator mc(pt, scene.camera, w, h, spp);

    ofPixels pix;
    mc.render(pix);

    std::string stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
    ofSaveImage(pix, "output/" + base + "_" +
                ofToString(captureCount) + stamp + "." + ext);
    ofLogNotice("ExportScene") << "frame saved";
}

void ExportScene::updateWindowTitle(float now)
{
    if (!exportTriggered) return;
    int p = 0;
    if (exportDuration > 0.f)
        p = static_cast<int>(std::clamp((now - startTime) /
                                        exportDuration, 0.f, 1.f) * 100.f + 0.5f);
    if (p != lastPercent)
    {
        ofSetWindowTitle("Exporting " + ofToString(p) + "%");
        lastPercent = p;
    }
}
