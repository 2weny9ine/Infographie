#pragma once
#include "ofMain.h"

class DragEventHandler {
public:
    DragEventHandler();
    void processDragEvent(const ofDragInfo& dragInfo);
};
