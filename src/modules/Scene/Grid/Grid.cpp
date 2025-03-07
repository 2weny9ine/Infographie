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

        // Retrieve and set grid size
        std::string gridSizeStr = Configuration::get("Grid.Grid Size");
        if (!gridSizeStr.empty())
        {
            setGridSize(std::stof(gridSizeStr));
            ofLog() << "Grid Size set to: " << gridSizeStr << endl;
        }
        else
        {
            ofLogError("Grid::update") << "Grid Size configuration is empty.";
        }

        // Retrieve and set primary line spacing
        std::string primaryLineSpacingStr = Configuration::get("Grid.Grid Spacing");
        if (!primaryLineSpacingStr.empty())
        {
            setPrimaryLineSpacing(std::stof(primaryLineSpacingStr));
            ofLog() << "Primary Line Spacing set to: " << primaryLineSpacingStr << endl;
        }
        else
        {
            ofLogError("Grid::update") << "Primary Line Spacing configuration is empty.";
        }

        // Retrieve and set secondary separations
        std::string secondarySeparationsStr = Configuration::get("Grid.Subdivisions");
        if (!secondarySeparationsStr.empty())
        {
            setSecondarySeparations(std::stoi(secondarySeparationsStr));
            ofLog() << "Secondary Separations set to: " << secondarySeparationsStr << endl;
        }
        else
        {
            ofLogError("Grid::update") << "Secondary Separations configuration is empty.";
        }

        // Retrieve and set primary grid color with opacity
        std::string primaryColorStr = Configuration::get("Grid.Primary Color");
        std::string primaryOpacityStr = Configuration::get("Grid.Primary Opacity");
        if (!primaryColorStr.empty() && !primaryOpacityStr.empty())
        {
            try
            {
                float primaryOpacity = std::stof(primaryOpacityStr);
                if (primaryColorStr[0] == '#')
                {
                    primaryColorStr = primaryColorStr.substr(1);
                }
                ofColor primaryColor = ofColor::fromHex(std::stoul(primaryColorStr, nullptr, 16));
                primaryColor.a = static_cast<int>(primaryOpacity * 255);
                setPrimaryGridColor(primaryColor);
                ofLog() << "Primary Color set to: " << primaryColorStr << " with opacity: " << primaryOpacityStr << endl;
            }
            catch (const std::exception& e)
            {
                ofLogError("Grid::update") << "Invalid hex color format for primary color: " << primaryColorStr << " - " << e.what();
            }
        }
        else
        {
            ofLogError("Grid::update") << "Primary Color or Opacity configuration is empty.";
        }

        // Retrieve and set secondary grid color with opacity
        std::string secondaryColorStr = Configuration::get("Grid.Secondary Color");
        std::string secondaryOpacityStr = Configuration::get("Grid.Secondary Opacity");
        if (!secondaryColorStr.empty() && !secondaryOpacityStr.empty())
        {
            try
            {
                float secondaryOpacity = std::stof(secondaryOpacityStr);
                if (secondaryColorStr[0] == '#')
                {
                    secondaryColorStr = secondaryColorStr.substr(1);
                }
                ofColor secondaryColor = ofColor::fromHex(std::stoul(secondaryColorStr, nullptr, 16));
                secondaryColor.a = static_cast<int>(secondaryOpacity * 255);
                setSecondaryGridColor(secondaryColor);
                ofLog() << "Secondary Color set to: " << secondaryColorStr << " with opacity: " << secondaryOpacityStr << endl;
            }
            catch (const std::exception& e)
            {
                ofLogError("Grid::update") << "Invalid hex color format for secondary color: " << secondaryColorStr << " - " << e.what();
            }
        }
        else
        {
            ofLogError("Grid::update") << "Secondary Color or Opacity configuration is empty.";
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("Grid::update") << "Error updating grid configuration: " << e.what();
    }
}

void Grid::draw()
{
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
    for (float z = -gridSize; z <= gridSize; z += secondaryLineSpacing)
    {
        ofDrawLine(-gridSize, 0, z, gridSize, 0, z);
    }

    for (float x = -gridSize; x <= gridSize; x += secondaryLineSpacing)
    {
        ofDrawLine(x, 0, -gridSize, x, 0, gridSize);
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
