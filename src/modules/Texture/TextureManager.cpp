#include "TextureManager.h"

TextureManager& TextureManager::get() {
    static TextureManager instance;
    return instance;
}

void TextureManager::setup() {
    if (!loaded) {
        loadTexture("wood", "textures/wood.jpg");
        loadTexture("tree", "textures/tree.jpg");
        loadTexture("brick", "textures/brick.jpg");
        loadTexture("checkerboard", "textures/checkerboard.jpg");

        dummyTexture.allocate(2, 2, GL_RGB);
        dummyTexture.clear();
        loaded = true;
    }
}

void TextureManager::loadTexture(const std::string& name, const std::string& path) {
    ofImage img;
    if (img.load(path)) {
        img.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
        textures[name] = img.getTexture();
        ofLogNotice("TextureManager") << "Loaded texture: " << name;
    }
    else {
        ofLogError("TextureManager") << "Failed to load texture: " << path;
    }
}

ofTexture& TextureManager::getTexture(const std::string& name) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    ofLogWarning("TextureManager") << "Requested unknown texture: " << name;
    return dummyTexture;
}
