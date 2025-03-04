#include "Grid.h"

Grid::Grid()
    : primaryGridSize(10000), secondaryGridSize(10000),
    primaryLineSpacing(25), secondaryLineSpacing(100),
    primaryGridColor(100, 100, 100, 100), secondaryGridColor(150, 150, 150, 150) {
}

void Grid::draw() {
    ofPushStyle();

    ofSetColor(primaryGridColor);
    ofSetLineWidth(1.0);
    drawPrimaryGrid();

    ofSetColor(secondaryGridColor);
    ofSetLineWidth(2.0);
    drawSecondaryGrid();

    ofPopStyle();
}

void Grid::drawPrimaryGrid() {
    for (int z = -primaryGridSize; z <= primaryGridSize; z += primaryLineSpacing) {
        ofDrawLine(-primaryGridSize, 0, z, primaryGridSize, 0, z);
    }

    for (int x = -primaryGridSize; x <= primaryGridSize; x += primaryLineSpacing) {
        ofDrawLine(x, 0, -primaryGridSize, x, 0, primaryGridSize);
    }
}

void Grid::drawSecondaryGrid() {
    for (int z = -secondaryGridSize; z <= secondaryGridSize; z += secondaryLineSpacing) {
        ofDrawLine(-secondaryGridSize, 0, z, secondaryGridSize, 0, z);
    }

    for (int x = -secondaryGridSize; x <= secondaryGridSize; x += secondaryLineSpacing) {
        ofDrawLine(x, 0, -secondaryGridSize, x, 0, secondaryGridSize);
    }
}
