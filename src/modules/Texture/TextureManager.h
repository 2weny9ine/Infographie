#pragma once
#include "ofMain.h"

class TextureManager {
public:
    static TextureManager& get();

    void setup();
    ofTexture& getCheckerboardTexture();

private:
    TextureManager() = default;

    ofImage checkerboardImage;
    ofTexture checkerboardTexture;
    bool loaded = false;
};
