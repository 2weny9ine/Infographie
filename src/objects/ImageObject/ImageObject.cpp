#include "ImageObject.h"

ImageObject::ImageObject() {
    ofLogNotice("ImageObject::ImageObject") << "ImageObject constructor called.";
}

ImageObject::~ImageObject() {
    ofLogNotice("ImageObject::~ImageObject") << "ImageObject destructor called.";
}


bool ImageObject::loadImage(const std::string& path) {
    ofLogNotice("ImageObject::loadImage") << "Loading image from path: " << path;
    bool success = image.load(path);
    if (success) {
        int imgWidth = image.getWidth();
        int imgHeight = image.getHeight();
        
        float maxCellWidth = 400.0f;
        float maxCellHeight = 300.0f;
        float scaleFactor = 1.0f;
        
        if (imgWidth > maxCellWidth || imgHeight > maxCellHeight) {
            scaleFactor = std::min(maxCellWidth / (float)imgWidth, maxCellHeight / (float)imgHeight);
        }
        

        plane.set(imgWidth * scaleFactor, imgHeight * scaleFactor, 2, 2);
        plane.mapTexCoords(0, 0, imgWidth, imgHeight);
    }
    else {
        ofLogError("ImageObject::loadImage") << "Failed to load image.";
    }
    return success;
}


void ImageObject::setup() {
    ofLogNotice("ImageObject::setup") << "Setting up ImageObject.";
}

void ImageObject::update(float dt) {
    ofLogNotice("ImageObject::update") << "Updating ImageObject with delta time: " << dt;
}

void ImageObject::draw() {
    ofPushMatrix();
    ofTranslate(position);
    
    if (image.isAllocated()) {
        image.getTexture().bind();
        plane.draw();
        image.getTexture().unbind();
    }
    else {
        ofLogWarning("ImageObject::draw") << "Image is not allocated.";
    }
    if (selected) {
        ofLogNotice("ImageObject::draw") << "Drawing bounding box for selected ImageObject.";
        drawBoundingBox();
    }
    ofPopMatrix();
}

void ImageObject::drawBoundingBox() {
    ofLogNotice("ImageObject::drawBoundingBox") << "Drawing bounding box.";
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofDrawRectangle(-plane.getWidth() / 2, -plane.getHeight() / 2, plane.getWidth(), plane.getHeight());
    ofSetColor(255);
}
