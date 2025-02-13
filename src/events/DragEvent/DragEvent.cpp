#include "DragEvent.h"
#include "objects/ImageObject/ImageObject.h"
#include "objects/ModelObject/ModelObject.h"

DragEvent::DragEvent() {
    ofLogNotice("DragEvent::DragEvent") << "DragEvent constructor called.";
}

DragEvent::~DragEvent() {
    ofLogNotice("DragEvent::~DragEvent") << "DragEvent destructor called.";
}

void DragEvent::processDragEvent(const ofDragInfo& dragInfo, Scene* scene) {
    ofLogNotice("DragEvent::processDragEvent") << "Processing drag event with " << dragInfo.files.size() << " files.";

    if (dragInfo.files.empty()) {
        ofLogWarning("DragEvent::processDragEvent") << "No files in drag event.";
        return;
    }

    for (const auto& file : dragInfo.files) {
        ofLogNotice("DragEvent::processDragEvent") << "Processing file: " << file;

        std::string extension = ofToLower(ofFilePath::getFileExt(file));
        ofLogNotice("DragEvent::processDragEvent") << "File extension: " << extension;

        if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "bmp") {
            ImageObject* imgObj = new ImageObject();
            ofLogNotice("DragEvent::processDragEvent") << "Attempting to load image: " << file;

            if (imgObj->loadImage(file)) {
                imgObj->position.set(0, 0, 0);
                scene->addObject(imgObj);
                ofLogNotice("DragEvent::processDragEvent") << "Image loaded and added to scene: " << file;
            }
            else {
                ofLogError("DragEvent::processDragEvent") << "Failed to load image: " << file;
                delete imgObj;
            }
        }
        else if (extension == "obj" || extension == "dae" || extension == "3ds") {
            ModelObject* modelObj = new ModelObject();
            ofLogNotice("DragEvent::processDragEvent") << "Attempting to load model: " << file;

            if (modelObj->loadModel(file)) {
                modelObj->position.set(0, 0, 0);
                scene->addObject(modelObj);
                ofLogNotice("DragEvent::processDragEvent") << "Model loaded and added to scene: " << file;
            }
            else {
                ofLogError("DragEvent::processDragEvent") << "Failed to load model: " << file;
                delete modelObj;
            }
        }
        else {
            ofLogWarning("DragEvent::processDragEvent") << "Unsupported file type: " << file;
        }
    }
}
