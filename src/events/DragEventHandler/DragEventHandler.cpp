#include "DragEventHandler.h"
#include "Application.h"

#include "objects/ImageObject/ImageObject.h"
#include "objects/3D/FileMesh/FileMesh.h"

DragEventHandler::DragEventHandler() {}

void DragEventHandler::processDragEvent(const ofDragInfo& dragInfo)
{
    Application& app = Application::getInstance();
    ofLogNotice("DragEventHandler::processDragEvent") << "Processing drag event with " << dragInfo.files.size() << " files.";

    if (dragInfo.files.empty())
    {
        ofLogWarning("DragEventHandler::processDragEvent") << "No files in drag event.";
        return;
    }

    for (const auto& file : dragInfo.files)
    {
        ofLogNotice("DragEventHandler::processDragEvent") << "Processing file: " << file;

        std::string extension = ofToLower(ofFilePath::getFileExt(file));
        ofLogNotice("DragEventHandler::processDragEvent") << "File extension: " << extension;

        if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "bmp")
        {
            ofLogNotice("DragEventHandler::processDragEvent") << "Attempting to load image: " << file;
            app.getScene().img->importImage(file);
        }
        else if (extension == "obj" || extension == "dae" || extension == "3ds", extension == "fbx")
        {
            FileMesh* modelObj = new FileMesh();
            ofLogNotice("DragEventHandler::processDragEvent") << "Attempting to load model: " << file;

            if (modelObj->loadModel(file))
            {
                app.getScene().addObject(modelObj);
                ofLogNotice("DragEventHandler::processDragEvent") << "Model loaded and added to scene: " << file;
            }
            else
            {
                ofLogError("DragEventHandler::processDragEvent") << "Failed to load model: " << file;
                delete modelObj;
            }
        }
        else
        {
            ofLogWarning("DragEventHandler::processDragEvent") << "Unsupported file type: " << file;
        }
    }
}
