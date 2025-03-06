#pragma once
#include "ofMain.h"

class MouseClickedEventHandler {
public:
    MouseClickedEventHandler();
    void handleMousePressed(int x, int y, int button);
    void handleMouseReleased(int x, int y, int button);
};
