#pragma once

#include "ofMain.h"
#include <vector>

enum class ConvolutionType {
    Identity,
    Sharpen,
    EdgeDetect,
    Blur
};

class ImageFilterManager {
public:
    void setup(ofTexture* originalTexture);
    void applyFilter(ConvolutionType type);
    ofTexture& getFilteredTexture();

private:
    std::vector<float> getKernel(ConvolutionType type);
    void convolve(const ofPixels& input, ofPixels& output, const std::vector<float>& kernel);

    ofTexture* sourceTexture = nullptr;
    ofTexture filteredTexture;
    ofImage sourceImage;
    ofImage filteredImage;
};
