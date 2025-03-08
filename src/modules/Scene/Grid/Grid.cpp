#include "Grid.h"
#include "modules/Configuration/Configuration.h"

Grid::Grid()
    : gridSize(10000),
    primaryLineSpacing(100), secondarySeparations(4),
    primaryGridColor(100, 100, 100, 100), secondaryGridColor(150, 150, 150, 150)
{
    this->update();
}

void Grid::update()
{
    try
    {
        ofLog() << "Updating grid configuration..." << endl;

        setGridSize(Configuration::getFloat("Grid.Grid Size"));
        ofLog() << "Grid Size set to: " << getGridSize() << endl;

        setPrimaryLineSpacing(Configuration::getFloat("Grid.Grid Spacing"));
        ofLog() << "Primary Line Spacing set to: " << getPrimaryLineSpacing() << endl;

        setSecondarySeparations(Configuration::getInt("Grid.Subdivisions"));
        ofLog() << "Secondary Separations set to: " << getSecondarySeparations() << endl;

        ofColor primaryColor = Configuration::getColor("Grid.Primary Color");
        float primaryOpacity = Configuration::getFloat("Grid.Primary Opacity");
        primaryColor.a = static_cast<int>(primaryOpacity * 255);
        setPrimaryGridColor(primaryColor);
        ofLog() << "Primary Color set to: " << primaryColor << " with opacity: " << primaryOpacity << endl;

        ofColor secondaryColor = Configuration::getColor("Grid.Secondary Color");
        float secondaryOpacity = Configuration::getFloat("Grid.Secondary Opacity");
        secondaryColor.a = static_cast<int>(secondaryOpacity * 255);
        setSecondaryGridColor(secondaryColor);
        ofLog() << "Secondary Color set to: " << secondaryColor << " with opacity: " << secondaryOpacity << endl;
    }
    catch (const std::exception& e)
    {
        ofLogError("Grid::update") << "Error updating grid configuration: " << e.what();
    }
}

void Grid::draw()
{
    if (!visible) return;

    ofPushStyle();

    ofSetColor(primaryGridColor);
    ofSetLineWidth(1.0);
    drawPrimaryGrid();

    ofSetColor(secondaryGridColor);
    ofSetLineWidth(4.0);
    drawSecondaryGrid();

    ofPopStyle();
}

void Grid::drawPrimaryGrid()
{
    for (float z = -gridSize; z <= gridSize; z += primaryLineSpacing)
    {
        ofDrawLine(-gridSize, 0, z, gridSize, 0, z);
    }

    for (float x = -gridSize; x <= gridSize; x += primaryLineSpacing)
    {
        ofDrawLine(x, 0, -gridSize, x, 0, gridSize);
    }
}

void Grid::drawSecondaryGrid()
{
    float secondaryLineSpacing = calculateSecondaryLineSpacing();
    float threshold = primaryLineSpacing * 0.05f;

    for (float z = -gridSize; z <= gridSize; z += secondaryLineSpacing)
    {
        if (fabs(fmod(z, primaryLineSpacing)) > threshold)
        {
            ofDrawLine(-gridSize, 0, z, gridSize, 0, z);
        }
    }

    for (float x = -gridSize; x <= gridSize; x += secondaryLineSpacing)
    {
        if (fabs(fmod(x, primaryLineSpacing)) > threshold)
        {
            ofDrawLine(x, 0, -gridSize, x, 0, gridSize);
        }
    }
}

float Grid::calculateSecondaryLineSpacing() const
{
    return primaryLineSpacing / (secondarySeparations + 1);
}

// Setters
void Grid::setGridSize(float size)
{
    gridSize = size;
}

void Grid::setPrimaryLineSpacing(float spacing)
{
    primaryLineSpacing = spacing;
}

void Grid::setSecondarySeparations(int separations)
{
    secondarySeparations = separations;
}

void Grid::setPrimaryGridColor(const ofColor& color)
{
    primaryGridColor = color;
}

void Grid::setSecondaryGridColor(const ofColor& color)
{
    secondaryGridColor = color;
}

void Grid::setVisible(bool v)
{
    visible = v;
}

// Getters
float Grid::getGridSize() const
{
    return gridSize;
}

float Grid::getPrimaryLineSpacing() const
{
    return primaryLineSpacing;
}

int Grid::getSecondarySeparations() const
{
    return secondarySeparations;
}

ofColor Grid::getPrimaryGridColor() const
{
    return primaryGridColor;
}

ofColor Grid::getSecondaryGridColor() const
{
    return secondaryGridColor;
}
