#pragma once
#include "ofMain.h"

class MouseEventHandler {
public:
    MouseEventHandler();
    void handleMousePressed(int x, int y, int button);
    void handleMouseReleased(int x, int y, int button);
    void handleMouseDragged(int x, int y, int button);
    void handleMouseMoved(int x, int y);

private:
    bool isRightMouseButtonDown;
};
