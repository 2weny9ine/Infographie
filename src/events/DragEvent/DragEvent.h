#pragma once

#include "ofMain.h"
#include "Scene.h"

class DragEvent {
public:
    DragEvent();
    ~DragEvent();

    void processDragEvent(const ofDragInfo& dragInfo, Scene* scene);
};
