#pragma once
#include "ofMain.h"

class Grid {
public:
    Grid();
    void draw();

private:
    int primaryGridSize;
    int secondaryGridSize;
    int primaryLineSpacing;
    int secondaryLineSpacing;
    ofColor primaryGridColor;
    ofColor secondaryGridColor;

    void drawPrimaryGrid();
    void drawSecondaryGrid();
};
