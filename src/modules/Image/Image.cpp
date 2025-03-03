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
    static float lastCaptureTime = 0.0f;
    float currentTime = ofGetElapsedTimef();
    

    if (captureCount == 0) {
        std::string timeStamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
        std::string fileName = name + "_0" + timeStamp + "." + extension;
        ofSaveScreen(fileName);

        captureCount++;
        lastCaptureTime = currentTime;
    }

    else if (captureCount < 5 && (currentTime - lastCaptureTime) >= 1.0f) {
        std::string timeStamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
        std::string fileName = name + "_" + ofToString(captureCount) + timeStamp + "." + extension;
        ofSaveScreen(fileName);
        ofLog() << "<export scene frame: " << fileName << ">";
        captureCount++;
        lastCaptureTime = currentTime;
    }
    

    if (captureCount >= 5) {
        captureCount = 0;
        exportTriggered = false;
    }
}


void Image::showImage() {
    if (images.empty()) return;
    
    int numImages = images.size();
    int cols = ceil(sqrt(numImages));
    int rows = ceil((float)numImages / cols);
    
    
    float spacingX = 600;
    float spacingY = 400;
    float depthZ = -800;
    
    
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

void Image::clearImages() {
    for (auto imgObj : images) {
        scene.removeObject(imgObj);
        delete imgObj;
    }
    images.clear();
}


void Image::createHistogram() {
    histogramR.assign(256, 0);
    histogramG.assign(256, 0);
    histogramB.assign(256, 0);

    if (images.empty()) return;

    ofPixels pixels = images[0]->getImage().getPixels();
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
    ofSetColor(30, 30, 30);
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




