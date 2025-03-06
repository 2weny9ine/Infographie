#include "ImageObject.h"

ImageObject::ImageObject(): applyUserColor(false), customUserColor(ofColor(255,255,255)) {
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
        
        float maxCellWidth = 800.0f;
        float maxCellHeight = 600.0f;
        float scaleFactor = 1.0f;
        
        if (imgWidth > maxCellWidth || imgHeight > maxCellHeight) {
            scaleFactor = std::min(maxCellWidth / (float)imgWidth, maxCellHeight / (float)imgHeight);
        }
        
        
        plane.set(imgWidth * scaleFactor, imgHeight * scaleFactor, 2, 2);
        plane.mapTexCoords(imgWidth, 0, 0, imgHeight);


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
    {
        ofTranslate(position);
        ofRotateXDeg(rotation.x);
        ofRotateYDeg(rotation.y);
        ofRotateZDeg(rotation.z);
        ofScale(scale.x, scale.y, scale.z);
        
        
        if(applyUserColor) {
            ofSetColor(customUserColor, 255);
        }
        else {
            ofSetColor(255, 255, 255);
        }
        
        if (image.isAllocated()) {
            image.getTexture().bind();
            plane.draw();
            image.getTexture().unbind();
        }
        
    
        if (selected) {
            drawBoundingBox();
        }
    }
    ofPopMatrix();
}

void ImageObject::drawBoundingBox() {
    ofLogNotice("ImageObject::drawBoundingBox") << "Drawing bounding box.";
    ofNoFill();
    ofSetColor(strokeColor); // Task 2.2: Use strokeColor
    ofSetLineWidth(lineWidth); // Task 2.2: Use lineWidth
    ofDrawRectangle(-plane.getWidth() / 2, -plane.getHeight() / 2, plane.getWidth(), plane.getHeight());  //2.2 ofSetColor(255);
}


//yacine
/**************************************************************************/
/**************************************************************************/
ofRectangle ImageObject::getScreenBoundingBox(ofCamera* cam) {
    glm::vec3 corners[4] = {
        glm::vec3(-plane.getWidth() / 2, -plane.getHeight() / 2, 0),
        glm::vec3(plane.getWidth() / 2, -plane.getHeight() / 2, 0),
        glm::vec3(plane.getWidth() / 2, plane.getHeight() / 2, 0),
        glm::vec3(-plane.getWidth() / 2, plane.getHeight() / 2, 0)
    };
    
    float minX = FLT_MAX, maxX = FLT_MIN, minY = FLT_MAX, maxY = FLT_MIN;
    for (auto& corner : corners) {
        glm::vec3 screenPos = cam->worldToScreen(corner + position);
        minX = std::min(minX, screenPos.x);
        maxX = std::max(maxX, screenPos.x);
        minY = std::min(minY, screenPos.y);
        maxY = std::max(maxY, screenPos.y);
    }
    return ofRectangle(minX, minY, maxX - minX, maxY - minY);
}
/**************************************************************************/
/**************************************************************************/


void ImageObject::applyFilter(const ofColor &filter) {
    applyUserColor = true;
    customUserColor = filter;
}


