#pragma once

#include "ofMain.h"
#include <vector>
#include <string>

enum class TextureFilterType {
    None,
    Emboss,
    Sharpen,
    EdgeDetect
};

class ImageFilterManager {
public:
    void setup(ofTexture* originalTexture);
    void applyFilter(TextureFilterType type);
    ofTexture& getFilteredTexture();

    TextureFilterType getCurrentFilterType() const;
    std::string getCurrentFilterName() const;

private:
    void applyConvolution(ofPixels& pixels, const std::vector<float>& kernel);

    ofTexture* originalTexture = nullptr;
    ofImage filteredImage;

    int imageWidth = 0;
    int imageHeight = 0;
    TextureFilterType currentFilter = TextureFilterType::None;

    const std::vector<float> convolution_kernel_identity = {
        0, 0, 0,
        0, 1, 0,
        0, 0, 0
    };

    const std::vector<float> convolution_kernel_emboss = {
        -2, -1, 0,
        -1,  1, 1,
         0,  1, 2
    };

    const std::vector<float> convolution_kernel_sharpen = {
         0, -1,  0,
        -1,  5, -1,
         0, -1,  0
    };

    const std::vector<float> convolution_kernel_edge_detect = {
         1,  0, -1,
         0,  0,  0,
        -1,  0,  1
    };
};