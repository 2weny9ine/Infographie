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
    static bool sequenceStarted = false;
    static float sequenceStartTime = 0.0f;
    static int captureCount = 0;
    float currentTime = ofGetElapsedTimef();
    if (!sequenceStarted) {
        sequenceStarted = true;
        sequenceStartTime = currentTime;
        captureCount = 0;
    }
    float elapsed = currentTime - sequenceStartTime;
    if (captureCount < 5 && elapsed >= captureCount + 1) {
        std::string timeStamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
        std::string fileName = name + "_" + ofToString(captureCount) + timeStamp + "." + extension;
        ofSaveScreen(fileName);
        ofLog() << "<export scene frame: " << fileName << ">";
        captureCount++;
    }
    if (captureCount >= 5) {
        sequenceStarted = false;
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
        
        ofLogNotice("showImage()")
        << "Image " << i << " => position("
        << x << ", " << y << ", " << z << ")";
    }
}

void Image::createHistogram() {
    histogramR.assign(256, 0);
    histogramG.assign(256, 0);
    histogramB.assign(256, 0);

    if (images.empty()) return;

    const auto& imgObject = images[0]; 
    const ofPixels& pixels = imgObject->getImage().getPixels();
    
    for (size_t i = 0; i < pixels.size(); i += pixels.getNumChannels()) {
        histogramR[pixels[i]]++;
        histogramG[pixels[i + 1]]++;
        histogramB[pixels[i + 2]]++;
    }
}


void Image::drawHistogram(int x, int y, int width, int height) {
    int maxVal = std::max({
        *std::max_element(histogramR.begin(), histogramR.end()),
        *std::max_element(histogramG.begin(), histogramG.end()),
        *std::max_element(histogramB.begin(), histogramB.end())
    });
    
    int barWidth = std::max(1, width / 256);
    
    ofPushMatrix();
    ofTranslate(x, y);
    ofSetColor(30, 30, 30);
    ofDrawRectangle(0, 0, width, height);
    
    for (int i = 0; i < 256; i++) {
        float barHeightR = ofMap(histogramR[i], 0, maxVal, 0, height);
        float barHeightG = ofMap(histogramG[i], 0, maxVal, 0, height);
        float barHeightB = ofMap(histogramB[i], 0, maxVal, 0, height);
        
        ofSetColor(255, 0, 0, 150);
        ofDrawRectangle(i * barWidth, height - barHeightR, barWidth, barHeightR);
        ofSetColor(0, 255, 0, 150);
        ofDrawRectangle(i * barWidth, height - barHeightG, barWidth, barHeightG);
        ofSetColor(0, 0, 255, 150);
        ofDrawRectangle(i * barWidth, height - barHeightB, barWidth, barHeightB);
    }
    ofPopMatrix();
}

