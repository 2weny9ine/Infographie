#pragma once
#include "ofMain.h"

class Image{
public:
    std::vector<ofImage> images;
    
    void importImage(const std::string& path);
    void imageExport(const std::string& name, const std::string& extension)const;
    void showImage();
};
