#include "TextureManager.h"

TextureManager& TextureManager::get() {
    static TextureManager instance;
    return instance;
}

void TextureManager::setup() {
    if (!loaded) {
        if (checkerboardImage.load("textures/wood.jpg")) {
            checkerboardTexture = checkerboardImage.getTexture();
            checkerboardTexture.setTextureWrap(GL_REPEAT, GL_REPEAT);
            loaded = true;
        }
        else {
            ofLogError("TextureManager") << "Failed to load wood texture!";
        }
    }
}

ofTexture& TextureManager::getCheckerboardTexture() {
    if (!loaded) setup(); 
    return checkerboardTexture;
}
