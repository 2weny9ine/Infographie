#pragma once
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofMain.h"
#include "Image.h"

#include <vector>

#include "objects/Object3D.h"
#include "Grid/Grid.h"
#include "modules/Cursor/Cursor.h"
#include "Top_Right_GUI.h"

#include "modules/BoundingBox/BoundingBox.h"

//2.3
/**************************************************************************/
/**************************************************************************/
struct Shape {
    PrimitiveType type;
    ofVec2f startPos;
    ofVec2f endPos;
    ofColor strokeColor;
    ofColor fillColor;
    float lineWidth;
    bool outline = false;
};
/**************************************************************************/
/**************************************************************************/

struct Locator {
    float position[3];
    float rotation[3];
    float proportion[3];
    bool isSelected;
};

class GUI;
class Image;
class IlluminationClassique;
class Scene {
public:
    enum class TransformMode {
        Translate, Rotate, Resize, None
    };

    Locator* locators;
    Image* img;
    ofNode node;
    GUI* gui;
    ofVec3f vector_position, vector_rotation, vector_proportion;
    ofVec3f lastMouseWorld;
    ofVec3f lastMouseScreen;

    ofCamera* camera;
    Grid* grid;

    TransformMode currentTransform;

    float center_x, center_y;
    float offset_x, offset_y, offset_z;
    float delta_x, delta_y, delta_z;
    float scaleFactor;
    float rotationSpeed;
    float speed;
    float planeZ;

    int radius;

    int locator_count;
    int locator_buffer_head;

    int mouse_press_x, mouse_press_y;
    int mouse_current_x, mouse_current_y;

    bool is_active_translation, is_active_rotation, is_active_proportion;
    bool is_mouse_button_pressed;

    //yacine
    /**************************************************************************/
    bool isDrawingMode = false; // New flag
    Cursor cursor;              // yacine 
    bool isMouseOverObject(int mouseX, int mouseY); // yacine
    /**************************************************************************/

    void dispatch_locators(int count, float range);
    void draw_locator(float scale);

    void resetSelection();
    void draw_zone(float x1, float y1, float x2, float y2) const;
    void selectAllInBounds(float x1, float y1, float x2, float y2);

    void apply_Transformations(ofVec3f position, ofVec3f rotation, ofVec3f scale);
    void update_Attributes();
    void duplicateSelectedInstances();

    void setup(ofCamera* cam, GUI* gui);
    void update();
    void draw();

    void addObject(Object3D* obj);
    void removeObject(Object3D* obj);

    Scene();
    ~Scene();

    ofVec3f calculProfondeur(const ofVec2f& pointEcran, float profondeur);

    //2.2
    /**************************************************************************/
    void updateDrawingProperties(const ofColor& stroke, const ofColor& fill, float width, bool outline);
    void drawCursor();
    //2.3
    void setPrimitiveType(PrimitiveType primitive);
    void startDrawing(int x, int y);
    void updateCurrentDrawing(int x, int y);
    void finalizeDrawing();
    void setOutlineEnabled(bool enabled);
    /**************************************************************************/

    std::vector<Object3D*> selectedObjects;
    std::vector<Object3D*> objects;

    void setNodeVisible(bool visible);
    void setIlluminationPtr(IlluminationClassique* ptr) { illumination = ptr; }
    void setMaterialPassEnabled(bool enabled) { materialPassEnabled = enabled; }
    
private:
    IlluminationClassique* illumination;
    bool materialPassEnabled = false; 
    // 2.3
    std::vector<Shape> shapes;
    Shape currentShape;
    bool isDrawing = false;
    PrimitiveType activePrimitive = PrimitiveType::RECTANGLE;
    void drawShape(const Shape& shape);
    bool outlineEnabled = false;

    bool nodeVisible = true;
private:
    BoundingBox boundingBoxAll;
    bool boundingBoxDirty = true;

    void updateBoundingBoxIfNeeded();
};
