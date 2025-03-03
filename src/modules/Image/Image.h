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
    void imageExport(const std::string& name, const std::string& extension);
    void showImage();
    void clearImages();
    void createHistogram();
    void drawHistogram(int x, int y, int width, int height);
    
    void setExportTriggered(bool triggered);
    bool hasImage() const { return !images.empty(); }
private:
    bool exportTriggered = false;
    
};

