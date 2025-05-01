#include "ImageObject.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>

ImageObject::ImageObject() : applyUserColor(false), customUserColor(ofColor(255, 255, 255))
{
    ofLogNotice("ImageObject::ImageObject") << "ImageObject constructor called.";
}

ImageObject::~ImageObject()
{
    ofLogNotice("ImageObject::~ImageObject") << "ImageObject destructor called.";
}

bool ImageObject::loadImage(const std::string& path)
{
    ofLogNotice("ImageObject::loadImage") << "Loading image from path: " << path;
    bool success = image.load(path);
    if (success)
    {
        int imgWidth = image.getWidth();
        int imgHeight = image.getHeight();

        float maxCellWidth = 800.0f;
        float maxCellHeight = 600.0f;
        float scaleFactor = 1.0f;

        if (imgWidth > maxCellWidth || imgHeight > maxCellHeight)
        {
            scaleFactor = std::min(maxCellWidth / (float)imgWidth, maxCellHeight / (float)imgHeight);
        }

        plane.set(imgWidth * scaleFactor, imgHeight * scaleFactor, 2, 2);
        plane.mapTexCoordsFromTexture(image.getTexture());

    }
    else
    {
        ofLogError("ImageObject::loadImage") << "Failed to load image.";
    }
    return success;
}

void ImageObject::setup()
{
    ofLogNotice("ImageObject::setup") << "Setting up ImageObject.";
    toneMappingShader.load("shaders/tone_mapping_330_vs.glsl", "shaders/tone_mapping_330_fs.glsl");
    if (toneMappingShader.isLoaded())
        ofLogNotice() << "Tone mapping shader loaded successfully.";
    else
        ofLogError() << "Failed to load tone mapping shader!";

}

void ImageObject::update(float dt)
{
    ofLogNotice("ImageObject::update") << "Updating ImageObject with delta time: " << dt;
}

void ImageObject::draw()
{
    ofPushMatrix();
    {
        ofTranslate(position);
        ofRotateXDeg(rotation.x);
        ofRotateYDeg(rotation.y);
        ofRotateZDeg(rotation.z);
        ofScale(scale.x, scale.y, scale.z);

        if (applyUserColor)
            ofSetColor(customUserColor, 255);
        else
            ofSetColor(255);

        if (image.isAllocated())
        {
            if (toneMappingEnabled && toneMappingShader.isLoaded()) {
                toneMappingShader.begin();
                toneMappingShader.setUniformTexture("image", image.getTexture(), 0);
                toneMappingShader.setUniform1f("tone_mapping_exposure", toneMappingExposure);
                toneMappingShader.setUniform1f("tone_mapping_gamma", toneMappingGamma);
                toneMappingShader.setUniform1i("tone_mapping_toggle", true); 
            }

            image.getTexture().bind();
            plane.draw();
            image.getTexture().unbind();

            if (toneMappingEnabled && toneMappingShader.isLoaded())
                toneMappingShader.end();
        }
    }
    ofPopMatrix();
}

// yacine
ofRectangle ImageObject::getScreenBoundingBox(ofCamera* cam)
{
    glm::vec3 corners[4] = {
        glm::vec3(-plane.getWidth() / 2, -plane.getHeight() / 2, 0),
        glm::vec3(plane.getWidth() / 2, -plane.getHeight() / 2, 0),
        glm::vec3(plane.getWidth() / 2,  plane.getHeight() / 2, 0),
        glm::vec3(-plane.getWidth() / 2,  plane.getHeight() / 2, 0)
    };

    float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX;
    for (auto& corner : corners)
    {
        glm::vec3 screenPos = cam->worldToScreen(corner + position);
        minX = std::min(minX, screenPos.x);
        maxX = std::max(maxX, screenPos.x);
        minY = std::min(minY, screenPos.y);
        maxY = std::max(maxY, screenPos.y);
    }
    return ofRectangle(minX, minY, maxX - minX, maxY - minY);
}

void ImageObject::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
{
    float w = plane.getWidth();
    float h = plane.getHeight();
    glm::vec3 localMin(-w * 0.5f, -h * 0.5f, 0);
    glm::vec3 localMax(w * 0.5f, h * 0.5f, 0);

    ofMatrix4x4 RS;
    RS.makeIdentityMatrix();
    RS.rotate(rotation.x, 1, 0, 0);
    RS.rotate(rotation.y, 0, 1, 0);
    RS.rotate(rotation.z, 0, 0, 1);
    RS.scale(scale.x, scale.y, scale.z);

    glm::vec3 corners[4] = {
        { localMin.x, localMin.y, 0 },
        { localMax.x, localMin.y, 0 },
        { localMax.x, localMax.y, 0 },
        { localMin.x, localMax.y, 0 }
    };

    glm::vec3 actualMin(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 actualMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    const float* m = RS.getPtr();
    for (auto& c : corners)
    {
        float x = c.x * m[0] + c.y * m[4] + c.z * m[8];
        float y = c.x * m[1] + c.y * m[5] + c.z * m[9];
        float z = c.x * m[2] + c.y * m[6] + c.z * m[10];
        glm::vec3 transformed(x, y, z);
        glm::vec3 worldCorner = transformed + position;
        actualMin = glm::min(actualMin, worldCorner);
        actualMax = glm::max(actualMax, worldCorner);
    }

    outMin = actualMin;
    outMax = actualMax;
}

void ImageObject::drawBoundingBox()
{
    glm::vec3 bbMin, bbMax;
    getWorldBounds(bbMin, bbMax);
    glm::vec3 center = (bbMin + bbMax) * 0.5f;
    float w = bbMax.x - bbMin.x;
    float h = bbMax.y - bbMin.y;
    float d = bbMax.z - bbMin.z;
    ofPushStyle();
    ofNoFill();
    ofSetColor(strokeColor);
    ofSetLineWidth(lineWidth);
    ofDrawBox(center, w, h, d);
    ofPopStyle();
}

void ImageObject::applyFilter(const ofColor& filter)
{
    applyUserColor = true;
    customUserColor = filter;
}

//6.3
std::vector<Property> ImageObject::getProperties() const {
    std::vector<Property> props = Object3D::getProperties();

    props.push_back(Property{
        "TM Exposure",
        PropertyType::Float,
        toneMappingExposure,
        0.0f, 5.0f, 2
        });

    props.push_back(Property{
        "TM Gamma",
        PropertyType::Float,
        toneMappingGamma,
        0.0f, 5.0f, 2
        });

    props.push_back(Property{
        "TM Toggle",
        PropertyType::Bool,
        toneMappingEnabled
        });

    return props;
}

//6.3
void ImageObject::setProperty(const Property& property) {
    if (property.name == "TM Exposure") {
        toneMappingExposure = std::get<float>(property.value);
    }
    else if (property.name == "TM Gamma") {
        toneMappingGamma = std::get<float>(property.value);
    }
    else if (property.name == "TM Toggle") {
        toneMappingEnabled = std::get<bool>(property.value);
    }
    else {
        Object3D::setProperty(property);
    }
}
