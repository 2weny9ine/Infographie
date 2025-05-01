#pragma once
#include "ofMain.h"
#include <unordered_map>

class TextureManager {
public:
    static TextureManager& get();

    void setup();
    ofTexture& getTexture(const std::string& name);

private:
    TextureManager() = default;

    std::unordered_map<std::string, ofTexture> textures;
    ofTexture dummyTexture;
    bool loaded = false;

    void loadTexture(const std::string& name, const std::string& path);
};
