#pragma once
#include "Scene.h"
#include "ImageObject.h"
#include "ofMain.h"

class Scene;

class Image{
public:
    std::vector<ImageObject*> images;
    
    Scene& scene;
    std::vector<int> histogramR, histogramG, histogramB;
    Image(Scene& scn) : scene(scn) {}
    
    
    void importImage(const std::string& path);
    void showImage();
    void clearImages();
    
    
    void setExportTriggered(bool triggered);
    void imageExport(const std::string& name, const std::string& extension);
    void setExportDuration(float duration);

    
    void createHistogram(ImageObject* selectedImage);
    void drawHistogram(int x, int y, int width, int height);
    void selectedHistogram(Scene &scene, int x, int y, int w, int h);
    
    void colorFilterSelected(Scene& scene, const ofColor& rgbColor, const ofColor& hsbColor);
    
   
    bool hasImage() const { return !images.empty(); }
private:
    bool exportTriggered = false;
    float exportDuration = 1.0f;

    void beforeExport();
    void afterExport();
};

