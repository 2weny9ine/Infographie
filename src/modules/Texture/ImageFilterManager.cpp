#include "ImageFilterManager.h"
#include "ofLog.h"
#include "ofGraphics.h"

void ImageFilterManager::setup(ofTexture* targetTexture) {
    originalTexture = targetTexture;
    currentFilter = TextureFilterType::None;
    imageWidth = originalTexture->getWidth();
    imageHeight = originalTexture->getHeight();

    filteredImage.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
    applyFilter(TextureFilterType::None);
}

void ImageFilterManager::applyFilter(TextureFilterType type) {
    if (!originalTexture) return;

    currentFilter = type;
    ofPixels pixels;
    originalTexture->readToPixels(pixels);

    switch (type) {
    case TextureFilterType::None:
        filteredImage.setFromPixels(pixels);
        break;
    case TextureFilterType::Emboss:
        applyConvolution(pixels, convolution_kernel_emboss);
        break;
    case TextureFilterType::Sharpen:
        applyConvolution(pixels, convolution_kernel_sharpen);
        break;
    case TextureFilterType::EdgeDetect:
        applyConvolution(pixels, convolution_kernel_edge_detect);
        break;
    }

    filteredImage.update();
    ofLog() << "[Filter] Applied: " << getCurrentFilterName();
}

void ImageFilterManager::applyConvolution(ofPixels& pixels, const std::vector<float>& kernel) {
    int w = pixels.getWidth();
    int h = pixels.getHeight();
    int channels = pixels.getNumChannels();

    const int kernelSize = 3;
    const int offset = kernelSize / 2;

    ofPixels src = pixels;
    ofPixels dst;
    dst.allocate(w, h, pixels.getPixelFormat());

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float sum[3] = { 0, 0, 0 };

            for (int j = -offset; j <= offset; ++j) {
                for (int i = -offset; i <= offset; ++i) {
                    int xi = ofClamp(x + i, 0, w - 1);
                    int yj = ofClamp(y + j, 0, h - 1);
                    ofColor color = src.getColor(xi, yj);
                    int kIndex = (j + offset) * kernelSize + (i + offset);

                    sum[0] += kernel[kIndex] * color.r;
                    sum[1] += kernel[kIndex] * color.g;
                    sum[2] += kernel[kIndex] * color.b;
                }
            }

            ofColor result;
            result.r = ofClamp(sum[0], 0, 255);
            result.g = ofClamp(sum[1], 0, 255);
            result.b = ofClamp(sum[2], 0, 255);

            dst.setColor(x, y, result);
        }
    }

    filteredImage.setFromPixels(dst);
}


ofTexture& ImageFilterManager::getFilteredTexture() {
    return filteredImage.getTexture();
}

TextureFilterType ImageFilterManager::getCurrentFilterType() const {
    return currentFilter;
}

std::string ImageFilterManager::getCurrentFilterName() const {
    switch (currentFilter) {
    case TextureFilterType::None: return "None";
    case TextureFilterType::Emboss: return "Emboss";
    case TextureFilterType::Sharpen: return "Sharpen";
    case TextureFilterType::EdgeDetect: return "Edge Detect";
    default: return "Unknown";
    }
}
