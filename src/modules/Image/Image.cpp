#include "Image.h"
#include "Scene.h"
#include "ImageObject.h"

void Image::importImage(const std::string& path) {
    
    ofImage img;
    auto imageObject = new ImageObject();
    if (imageObject->loadImage(path)) {
        images.push_back(imageObject);
        scene.addObject(imageObject);
        
        showImage();
    } else {
        delete imageObject;
        ofLog() << "Erreur l'image : " << path;
    }
    
    
}

void Image::setExportTriggered(bool triggered) {
    exportTriggered = triggered;
}

void Image::imageExport(const std::string& name, const std::string& extension) {
    if (!exportTriggered) return;
    
    static int captureCount = 0;
    static float startTime = 0.0f;
    static float lastCaptureTime = 0.0f;
    float currentTime = ofGetElapsedTimef();
    
    if (captureCount == 0) {
        startTime = currentTime;
        lastCaptureTime = currentTime;
    }
    
    
    if ((currentTime - startTime) < exportDuration) {
        
        if (currentTime - lastCaptureTime >= 1.0f || captureCount == 0) {
            std::string timeStamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
            std::string fileName = "output/" + name + "_" + ofToString(captureCount) + timeStamp + "." + extension;
            ofSaveScreen(fileName);
            ofLog() << "<export scene frame: " << fileName << ">";
            captureCount++;
            lastCaptureTime = currentTime;
        }
    } else {
        
        captureCount = 0;
        exportTriggered = false;
    }
}

void Image::setExportDuration(float duration) {
    exportDuration = duration;
}



void Image::showImage() {
    if (images.empty()) return;
    
    int numImages = images.size();
    int cols = ceil(sqrt(numImages));
    int rows = ceil((float)numImages / cols);
    
    
    float spacingX = 1000;
    float spacingY = 700;
    float depthZ = 0;
    
    
    float startX = -((cols - 1) * spacingX) / 2.0f;
    float startY =  ((rows - 1) * spacingY) / 2.0f;
    
    for (int i = 0; i < numImages; i++) {
        int col = i % cols;
        int row = i / cols;
        
        float x = startX + col * spacingX;
        float y = startY - row * spacingY;
        float z = depthZ;
        
        images[i]->setPosition({x, y, z});
        
    }
}




void Image::createHistogram(ImageObject* selectedImage) {
    histogramR.assign(256, 0);
    histogramG.assign(256, 0);
    histogramB.assign(256, 0);
    
    if (images.empty()) return;
    
    ofPixels pixels = selectedImage->getImage().getPixels();
    int channels = pixels.getNumChannels();
    
    for (int i = 0; i < pixels.size(); i += channels) {
        histogramR[pixels[i]]++;
        histogramG[pixels[i + 1]]++;
        histogramB[pixels[i + 2]]++;
    }
}



void Image::drawHistogram(int x, int y, int width, int height) {
    int maxVal = 0;
    for (int i = 0; i < 256; i++) {
        if (histogramR[i] > maxVal) maxVal = histogramR[i];
        if (histogramG[i] > maxVal) maxVal = histogramG[i];
        if (histogramB[i] > maxVal) maxVal = histogramB[i];
    }
    
    int bw = width / 256;
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(50, 50, 50);
    ofDrawRectangle(0, 0, width, height);
    
    for (int i = 0; i < 256; i++) {
        float rH = (histogramR[i] * height) / (float)maxVal;
        float gH = (histogramG[i] * height) / (float)maxVal;
        float bH = (histogramB[i] * height) / (float)maxVal;
        
        ofSetColor(255, 0, 0, 100);
        ofDrawRectangle(i * bw, height - rH, bw, rH);
        
        ofSetColor(0, 255, 0, 100);
        ofDrawRectangle(i * bw, height - gH, bw, gH);
        
        ofSetColor(0, 0, 255, 100);
        ofDrawRectangle(i * bw, height - bH, bw, bH);
    }
    ofPopMatrix();
}

void Image::selectedHistogram(Scene &scene, int x, int y, int w, int h)
{
    ofPushStyle();
    ofSetColor(50, 50, 50);
    ofDrawRectangle(x, y, w, h);
    ofSetColor(255);
    
    if (scene.selectedObjects.size() != 1) {
        ofDrawBitmapString("Selectionner une image!", x + 10, y + h / 2);
        return;
    }
    auto *selectedImg = dynamic_cast<ImageObject*>(scene.selectedObjects[0]);
    if (!selectedImg) {
        ofDrawBitmapString("Selectionner une image!", x + 10, y + h / 2);
        return;
    }
    
    createHistogram(selectedImg);
    drawHistogram(x, y, w, h);
    ofPopStyle();
}

void Image::colorFilterSelected(Scene& scene, const ofColor& rgbColor, const ofColor& hsbColor)
{
    ofColor combinedColor;
    combinedColor.r = (rgbColor.r + hsbColor.r) / 2;
    combinedColor.g = (rgbColor.g + hsbColor.g) / 2;
    combinedColor.b = (rgbColor.b + hsbColor.b) / 2;
    combinedColor.a = 255;
    
    std::vector<ImageObject*> selectedImages;
    for (auto *obj : scene.selectedObjects) {
        if (ImageObject *imgObj = dynamic_cast<ImageObject*>(obj)) {
            selectedImages.push_back(imgObj);
        }
    }

    
    if(selectedImages.size() >= 1){
        for (auto *imgObj : selectedImages) {
            imgObj->applyFilter(combinedColor);
        }
    }
}







