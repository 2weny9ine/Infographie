// Image.cpp
#include "Image.h"
#include "Scene.h"
#include "ImageObject.h"
#include "Application.h"
#include "modules/Configuration/Configuration.h"
#include "ofMain.h"
#include <cmath>

Image::Image(Scene& scn)
    : scene(scn)
    , exporter(scn)
{}

void Image::importImage(const std::string& path)
{
    auto* imageObject = new ImageObject();
    if (imageObject->loadImage(path))
    {
        images.push_back(imageObject);
        scene.addObject(imageObject);
        showImage();
    }
    else
    {
        delete imageObject;
        ofLogError("Image::importImage") << "Impossible de charger l'image : " << path;
    }
}

void Image::showImage()
{
    if (images.empty()) return;

    int numImages = static_cast<int>(images.size());
    int cols = static_cast<int>(std::ceil(std::sqrt(numImages)));
    int rows = static_cast<int>(std::ceil((float)numImages / cols));

    float spacingX = 1000;
    float spacingY = 700;
    float depthZ = 0;

    float startX = -((cols - 1) * spacingX) / 2.0f;
    float startY = ((rows - 1) * spacingY) / 2.0f;

    for (int i = 0; i < numImages; ++i)
    {
        int col = i % cols;
        int row = i / cols;
        float x = startX + col * spacingX;
        float y = startY - row * spacingY;
        images[i]->setPosition({ x, y, depthZ });
    }
}

void Image::clearImages()
{
    for (auto* imgObj : images)
    {
        scene.removeObject(imgObj);
        delete imgObj;
    }
    images.clear();
    histogramR.clear();
    histogramG.clear();
    histogramB.clear();
}

void Image::createHistogram(ImageObject* selectedImage)
{
    histogramR.assign(256, 0);
    histogramG.assign(256, 0);
    histogramB.assign(256, 0);

    auto& img = selectedImage->getImage();
    auto  px = img.getPixels();
    int   ch = px.getNumChannels();

    for (std::size_t i = 0; i < px.size(); i += ch)
    {
        histogramR[px[i]]++;
        histogramG[px[i + 1]]++;
        histogramB[px[i + 2]]++;
    }
}

void Image::drawHistogram(int x, int y, int width, int height)
{
    if (histogramR.empty()) return;

    int maxVal = 0;
    for (int i = 0; i < 256; ++i)
    {
        maxVal = std::max({ maxVal, histogramR[i], histogramG[i], histogramB[i] });
    }

    int bw = width / 256;
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(50);
    ofDrawRectangle(0, 0, width, height);

    for (int i = 0; i < 256; ++i)
    {
        float rH = histogramR[i] * height / (float)maxVal;
        float gH = histogramG[i] * height / (float)maxVal;
        float bH = histogramB[i] * height / (float)maxVal;

        ofSetColor(255, 0, 0, 100);
        ofDrawRectangle(i * bw, height - rH, bw, rH);

        ofSetColor(0, 255, 0, 100);
        ofDrawRectangle(i * bw, height - gH, bw, gH);

        ofSetColor(0, 0, 255, 100);
        ofDrawRectangle(i * bw, height - bH, bw, bH);
    }
    ofPopMatrix();
}

void Image::selectedHistogram(Scene& scene, int x, int y, int w, int h)
{
    ofPushStyle();
    ofSetColor(50);
    ofDrawRectangle(x, y, w, h);
    ofSetColor(255);

    if (scene.selectedObjects.size() != 1)
    {
        ofDrawBitmapString("Sélectionnez une image !", x + 10, y + h / 2);
        ofPopStyle();
        return;
    }

    auto* sel = dynamic_cast<ImageObject*>(scene.selectedObjects[0]);
    if (!sel)
    {
        ofDrawBitmapString("Sélectionnez une image !", x + 10, y + h / 2);
        ofPopStyle();
        return;
    }

    createHistogram(sel);
    drawHistogram(x, y, w, h);
    ofPopStyle();
}

void Image::colorFilterSelected(Scene& scene,
                                const ofColor& rgbColor,
                                const ofColor& hsbColor)
{
    ofColor combined;
    combined.r = (rgbColor.r + hsbColor.r) / 2;
    combined.g = (rgbColor.g + hsbColor.g) / 2;
    combined.b = (rgbColor.b + hsbColor.b) / 2;
    combined.a = 255;

    for (auto* obj : scene.selectedObjects)
    {
        if (auto* imgObj = dynamic_cast<ImageObject*>(obj))
        {
            imgObj->applyFilter(combined);
        }
    }
}

void Image::triggerExport(bool triggered)
{
    exporter.setExportTriggered(triggered);
}

void Image::setExportDuration(float duration)
{
    exporter.setExportDuration(duration);
}

void Image::exportFrames(const std::string& name,
                         const std::string& extension)
{
    exporter.exportFrames(name, extension);
}

void Image::imageExport(const std::string& name,
                        const std::string& extension)
{
    exporter.exportFrames(name, extension);
}

