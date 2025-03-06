#pragma once
#include "ofMain.h"
#include <unordered_set>

class KeyEventHandler {
public:
    KeyEventHandler();
    void handleKeyPressed(int key);
    void handleKeyReleased(int key);
    void update();

private:
    bool ctrlPressed;
    bool duplicated;
    std::unordered_set<int> pressedKeys;
};
