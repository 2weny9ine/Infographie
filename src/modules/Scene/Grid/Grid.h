#pragma once
#include "ofMain.h"
#include "modules/Configuration/Configuration.h"

class Grid {
public:
    Grid();
    void draw();
    void update();

    // Setters
    void setGridSize(float size);
    void setPrimaryLineSpacing(float spacing);
    void setSecondarySeparations(int separations);
    void setPrimaryGridColor(const ofColor& color);
    void setSecondaryGridColor(const ofColor& color);
    void setVisible(bool visible);

    // Getters
    float getGridSize() const;
    float getPrimaryLineSpacing() const;
    int getSecondarySeparations() const;
    ofColor getPrimaryGridColor() const;
    ofColor getSecondaryGridColor() const;

private:
    float gridSize;
    float primaryLineSpacing;
    int secondarySeparations;
    ofColor primaryGridColor;
    ofColor secondaryGridColor;
    bool visible;

    void drawPrimaryGrid();
    void drawSecondaryGrid();
    float calculateSecondaryLineSpacing() const;
};
