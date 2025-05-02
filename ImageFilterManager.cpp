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

    ofPixels src = pixels;
    ofPixels dst = pixels;
    dst.setColor(ofColor::black); // initialize

    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            for (int c = 0; c < channels; ++c) {
                float sum = 0.0f;
                int idx = 0;

                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int sx = x + kx;
                        int sy = y + ky;
                        sum += kernel[idx++] * src.getColor(sx, sy)[c];
                    }
                }

                int finalValue = ofClamp(sum, 0, 255);
                ofColor col = dst.getColor(x, y);
                col[c] = finalValue;
                dst.setColor(x, y, col);
            }
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
