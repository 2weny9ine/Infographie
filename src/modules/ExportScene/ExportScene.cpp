// modules/ExportScene/ExportScene.cpp
#include "ExportScene.h"
#include "Scene.h"
#include "modules/Configuration/Configuration.h"
#include "modules/Raytracing/Raytracing.h"
#include "Object3D.h"
#include "enums/SurfaceType.h"
#include "ofMain.h"
#include <algorithm>

ExportScene::ExportScene(Scene& scn)
    : scene(scn)
{}

void ExportScene::setExportTriggered(bool triggered)
{
    exportTriggered = triggered;
    if (exportTriggered)
    {
        captureCount = 0;
        startTime = ofGetElapsedTimef();
        lastCaptureTime = startTime;
        beforeExport();
    }
}

void ExportScene::setExportDuration(float duration)
{
    exportDuration = duration;
}

void ExportScene::exportFrames(const std::string& name,
                               const std::string& extension)
{
    if (!exportTriggered) return;

    float now = ofGetElapsedTimef();
    float elapsed = now - startTime;

    if (elapsed < exportDuration)
    {
        if (captureCount == 0 || (now - lastCaptureTime) >= 1.0f)
        {
            // Check for any mirror spheres
            bool hasMirror = std::any_of(
                scene.objects.begin(), scene.objects.end(),
                [](Object3D* obj)
            {
                return obj->getSurfaceType() == SurfaceType::MIRROR;
            }
            );

            if (hasMirror)
            {
                renderRaytracedFrame(name, extension);
            }
            else
            {
                // fallback to normal capture
                std::string stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
                std::string file = "output/" + name
                    + "_" + ofToString(captureCount)
                    + stamp + "." + extension;
                ofSaveScreen(file);
                ofLogNotice("ExportScene") << "Simple export: " << file;
            }

            captureCount++;
            lastCaptureTime = now;
        }
    }
    else
    {
        exportTriggered = false;
        afterExport();
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

void ExportScene::renderRaytracedFrame(const std::string& name,
                                       const std::string& extension)
{
    int w = ofGetWidth();
    int h = ofGetHeight();
    ofPixels pix;
    pix.allocate(w, h, OF_PIXELS_RGB);

    Raytracing tracer;
    for (auto* obj : scene.objects)
    {
        tracer.addObject(obj);
    }

    ofCamera* cam = scene.camera;
    float nearZ = cam->getNearClip();
    float farZ = cam->getFarClip();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            glm::vec3 wpNear = cam->screenToWorld({ (float)x,(float)y,nearZ });
            glm::vec3 wpFar = cam->screenToWorld({ (float)x,(float)y,farZ });
            glm::vec3 dir = glm::normalize(wpFar - wpNear);
            Ray ray(wpNear, dir);

            Intersection isect;
            ofColor col(0);

            if (tracer.trace(ray, isect))
            {
                Object3D* hit = isect.object;
                if (hit->getSurfaceType() == SurfaceType::MIRROR)
                {
                    glm::vec3 R = glm::reflect(ray.direction, isect.normal);
                    Ray r2(isect.point + R * 1e-4f, R);
                    Intersection is2;
                    if (tracer.trace(r2, is2))
                    {
                        col = is2.object->getColor();
                    }
                }
                else
                {
                    col = hit->getColor();
                }
            }
            pix.setColor(x, h - 1 - y, col);
        }
    }

    std::string stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
    std::string file = "output/" + name
        + "_" + ofToString(captureCount)
        + stamp + "." + extension;

    ofSaveImage(pix, file);
    ofLogNotice("ExportScene") << "Raytraced export: " << file;
}
