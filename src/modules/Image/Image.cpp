#include "Image.h"

void Image::importImage(const std::string& path) {

    ofImage img;
    if (img.load(path)) {
        images.push_back(img);
    } else {
        ofLog() << "Erreur l'image : " << path;
    }
    
    
}

void Image::imageExport(const std::string&  name, const std::string& extension)const
{
    
    string timeStamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
    
    string fileName = name + timeStamp + "." + extension;
    
    for(size_t i=0; i< images.size();i++){
        string fileName = name + std::to_string(i) + timeStamp + "." + extension;
        images[i].save(fileName);
    }
    
     
    ofLog() << "<export image: " << fileName << ">";
}

void Image::showImage()
{
    if (images.empty()) return;

    int windowWidth = ofGetWidth();
    int windowHeight = ofGetHeight();
    int numImages = images.size();

    if (numImages == 1) {
        images[0].draw(0, 0, windowWidth, windowHeight);
    }
    else {
        int cols = ceil(sqrt(numImages));
        int rows = ceil((float)numImages / cols);

        int targetWidth = windowWidth / cols;
        int targetHeight = windowHeight / rows;

        for (int i = 0; i < numImages; i++) {
            int x = (i % cols) * targetWidth;
            int y = (i / cols) * targetHeight;
            
            images[i].draw(x, y, targetWidth, targetHeight);
        }
    }
}


