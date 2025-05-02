#include "Scene.h"
#include "Image.h"
#include "IlluminationClassique.h"  
#include "modules/IlluminationModerne/IlluminationModerne.h"  
#include "ofMain.h"
#include "GUI.h"
#include "primitiveObject.h"
#include "Topologie.h"

void Scene::setup(ofCamera* cam, GUI* gui)
{
    grid = new Grid();
    this->gui = gui;
    camera = cam;
    
    locator_count = 100;
    speed = 100.0f;
    radius = 3.0f;
    
    mouse_press_x = 0;
    mouse_press_y = 0;
    mouse_current_x = 0;
    mouse_current_y = 0;
    
    is_mouse_button_pressed = false;
    is_active_translation = false;
    is_active_rotation = false;
    is_active_proportion = false;
    
    offset_x = 0.0f;
    offset_y = 0.0f;
    offset_z = 0.0f;
    
    delta_x = speed;
    delta_y = speed;
    delta_z = speed;
    
    locator_buffer_head = 0;
    
    cursor.setup();
    
    topologie = new Topologie(this);
    topologie->setup();
    
    boundingBoxDirty = true;
}

void Scene::update()
{
    center_x = ofGetWidth() / 2.0f;
    center_y = ofGetHeight() / 2.0f;
    
    ofVec3f mouseScreen(ofGetMouseX(), ofGetMouseY(), 0);
    
    if (!isDrawing)
    {
        if (currentTransform == Scene::TransformMode::Translate)
        {
            if (is_mouse_button_pressed)
            {
                ofVec3f delta = mouseScreen - lastMouseScreen;
                
                ofVec3f right = camera->getXAxis(); // camera X axis
                ofVec3f up = camera->getYAxis(); // camera Y axis
                float sensitivity = 0.01f;
                
                for (Object3D* object : selectedObjects)
                {
                    object->transformPosition(right * delta.x * sensitivity);
                    object->transformPosition(up * -delta.y * sensitivity);
                }
                
                gui->bottom_left->localTransformations[0] += delta.x;
                gui->bottom_left->localTransformations[1] += delta.y;
                gui->bottom_left->localTransformations[2] += delta.z;
                
                boundingBoxDirty = true;
                
                update_Attributes();
            }
            
            cursor.setState(CursorState::MOVE);
            lastMouseScreen = mouseScreen;
        }
        else if (currentTransform == Scene::TransformMode::Resize)
        {
            if (is_mouse_button_pressed)
            {
                ofVec2f mouseDelta = -(mouseScreen - lastMouseScreen);
                
                for (Object3D* object : selectedObjects)
                {
                    ofVec3f newScale = object->getScale();
                    newScale.x += mouseDelta.y * scaleFactor;
                    newScale.y += mouseDelta.y * scaleFactor;
                    newScale.z += mouseDelta.y * scaleFactor;
                    newScale.x = max(1.0f, newScale.x);
                    newScale.y = max(1.0f, newScale.y);
                    newScale.z = max(1.0f, newScale.z);
                    object->setScale(newScale);
                }
                
                gui->bottom_left->localTransformations[6] += mouseDelta.y * scaleFactor;
                gui->bottom_left->localTransformations[7] += mouseDelta.y * scaleFactor;
                gui->bottom_left->localTransformations[8] += mouseDelta.y * scaleFactor;
                
                boundingBoxDirty = true;
                
                update_Attributes();
            }
            
            cursor.setState(CursorState::RESIZE);
            lastMouseScreen = mouseScreen;
        }
        else if (currentTransform == Scene::TransformMode::Rotate)
        {
            if (is_mouse_button_pressed)
            {
                ofVec2f mouseDelta = mouseScreen - lastMouseScreen;
                
                for (Object3D* object : selectedObjects)
                {
                    ofVec3f delta = ofVec3f(mouseDelta.y * rotationSpeed,
                                            mouseDelta.x * rotationSpeed,
                                            (mouseDelta.x + mouseDelta.y) * rotationSpeed * 0.5f);
                    
                    object->transformRotation(delta);
                }
                
                gui->bottom_left->localTransformations[3] += mouseDelta.y * rotationSpeed;
                gui->bottom_left->localTransformations[4] -= mouseDelta.x * rotationSpeed;
                gui->bottom_left->localTransformations[5] += (mouseDelta.x + mouseDelta.y) * rotationSpeed * 0.5f;
                
                boundingBoxDirty = true;
                
                update_Attributes();
            }
            cursor.setState(CursorState::ROTATE);
            lastMouseScreen = mouseScreen;
        }
    }
    
    cursor.update(mouse_current_x, mouse_current_y, is_mouse_button_pressed);

    //img->update();
    topologie->update( ofGetLastFrameTime() );

}

Scene::Scene()
{
    img = new Image(*this);
    currentTransform = TransformMode::None;
    scaleFactor = 0.2f;
    rotationSpeed = 0.5f;
    planeZ = 0.0f;
    topologie = new Topologie(this);
    boundingBoxDirty = true;
}

void Scene::draw()
{
    camera->begin();
    ofPushMatrix();

    grid->draw();
    glEnable(GL_DEPTH_TEST);
    ofDisableArbTex();

    if (illuminationClassique && illuminationClassique->getMode() != IlluminationClassique::Mode::AUCUN && !illuminationModerne->activated) {
        illuminationClassique->draw();
    }
    else if (illuminationModerne && illuminationModerne->activated)
    {
        illuminationModerne->draw();
    }
    else {
        for (auto* obj : objects) {
            obj->draw();
        }
    }
    
    topologie->draw();

    if (materialPassEnabled && !selectedObjects.empty() && illuminationClassique && illuminationClassique->getMode() != IlluminationClassique::Mode::AUCUN && !illuminationModerne->activated) {
        illuminationClassique->renderMaterialPass();
    }
    else if (materialPassEnabled && !selectedObjects.empty() && illuminationModerne && illuminationModerne->activated) {
        illuminationModerne->renderMaterialPass();
    }
    glDisable(GL_DEPTH_TEST);

    if (!selectedObjects.empty()) {
        updateBoundingBoxIfNeeded();
        boundingBoxAll.draw();
    }

    draw_locator(10.0f);

    ofPopMatrix();
    camera->end();

    if (is_mouse_button_pressed && currentTransform == TransformMode::None) {
        draw_zone(mouse_press_x, mouse_press_y, mouse_current_x, mouse_current_y);
    }

    //2.3
    for (auto& shape : shapes) drawShape(shape);
    if (isDrawing) drawShape(currentShape);
    //cursor.draw();

    //img->draw();
}



void Scene::updateBoundingBoxIfNeeded()
{
    if (boundingBoxDirty)
    {
        boundingBoxAll.update(selectedObjects);
        boundingBoxDirty = false;
    }
}

//2.2
void Scene::drawCursor()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ofDisableDepthTest();
    cursor.draw();
    glPopAttrib();
}

bool Scene::isMouseOverObject(int mouseX, int mouseY)
{
    for (auto obj : objects)
    {
        ofRectangle bbox = obj->getScreenBoundingBox(camera);
        if (bbox.inside(mouseX, mouseY))
        {
            return true;
        }
    }
    return false;
}

void Scene::draw_zone(float x1, float y1, float x2, float y2) const
{
    float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
    float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());
    ofSetLineWidth(radius);
    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
    ofFill();
    ofDrawEllipse(x1, y1, radius, radius);
    ofDrawEllipse(x1, y2_clamp, radius, radius);
    ofDrawEllipse(x2_clamp, y1, radius, radius);
    ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
}

void Scene::selectAllInBounds(float x1, float y1, float x2, float y2)
{
    resetSelection();
    float minX = min(x1, x2);
    float maxX = max(x1, x2);
    float minY = min(y1, y2);
    float maxY = max(y1, y2);
    
    for (Object3D* object : objects)
    {
        ofVec3f objectPos = object->getPosition();
        glm::vec3 screenPos = camera->worldToScreen(objectPos);
        
        if (screenPos.x >= minX && screenPos.x <= maxX &&
            screenPos.y >= minY && screenPos.y <= maxY)
        {
            object->setSelected(true);
            selectedObjects.push_back(object);
        }
        else
        {
            object->setSelected(false);
        }
    }
    gui->bottom_left->updatePropertyControls();
    
    boundingBoxDirty = true;
}

void Scene::update_Attributes()
{
    gui->bottom_left->updatePropertyControls();
    boundingBoxDirty = true;
    updateBoundingBoxIfNeeded();
}



void Scene::apply_Transformations(ofVec3f position, ofVec3f rotation, ofVec3f scale)
{
    if (selectedObjects.size() == 1)
    {
        selectedObjects[0]->setPosition(position);
        selectedObjects[0]->setRotation(rotation);
        selectedObjects[0]->setScale(scale);
    }
    else if (selectedObjects.size() > 1)
    {
        for (Object3D* object : selectedObjects)
        {
            object->transformPosition(position);
            object->transformRotation(rotation);
            object->transformScale(scale);
        }
    }
    
    boundingBoxDirty = true;
    updateBoundingBoxIfNeeded();
}

void Scene::duplicateSelectedInstances()
{
    std::vector<Object3D*> newCopies;
    
    for (Object3D* object : selectedObjects)
    {
        Object3D* copy = object->copy();
        copy->transformPosition(ofVec3f(100, 0, 100));
        newCopies.push_back(copy);
        
        this->addObject(copy);
    }
    
    this->resetSelection();
    
    for (Object3D* object : newCopies)
    {
        object->setSelected(true);
        selectedObjects.push_back(object);
    }
    
    boundingBoxDirty = true;
}

void Scene::draw_locator(float scale)
{
    if (!nodeVisible) return;
    
    ofSetLineWidth(4);
    ofSetColor(127);
    ofFill();
    ofPushMatrix();
    if (selectedObjects.size() > 0)
    {
        // Set locator to centroid
        ofVec3f centroid(0, 0, 0);
        for (Object3D* object : selectedObjects)
        {
            centroid += object->getPosition();
        }
        node.setPosition(centroid / selectedObjects.size());
    }
    else
    {
        node.setPosition(0, 0, 0);
    }
    
    node.setOrientation(ofVec3f(0, 0, 0));
    node.setScale(scale);
    node.draw();
    ofPopMatrix();
}

void Scene::setNodeVisible(bool visible)
{
    this->nodeVisible = visible;
}

void Scene::resetSelection()
{
    for (Object3D* object : selectedObjects)
        object->setSelected(false);
    
    selectedObjects.clear();
    gui->bottom_left->localTransformations = { 0,0,0,0,0,0,0,0,0 };
    
    boundingBoxDirty = true;
}

void Scene::addObject(Object3D* obj)
{
    objects.push_back(obj);
    gui->top_right.get()->addObjectToggle(obj);
    ofLogNotice("Scene::addObject") << "Object added to scene.";
    boundingBoxDirty = true;
}

void Scene::removeObject(Object3D* obj)
{
    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it != objects.end())
    {
        objects.erase(it);
    }
    boundingBoxDirty = true;
}

// 2.2
void Scene::setOutlineEnabled(bool enabled)
{
    outlineEnabled = enabled;
}

void Scene::updateDrawingProperties(const ofColor& stroke, const ofColor& fill, float width, bool outline)
{
    currentShape.strokeColor = stroke;
    currentShape.fillColor = fill;
    currentShape.lineWidth = width;
    currentShape.outline = outline;
}

void Scene::setPrimitiveType(PrimitiveType primitive)
{
    activePrimitive = primitive;
}

void Scene::startDrawing(int x, int y)
{
    isDrawing = true;
    currentShape.type = activePrimitive;
    currentShape.startPos.set(x, y);
    currentShape.endPos.set(x, y);
    currentShape.outline = outlineEnabled;
}

void Scene::updateCurrentDrawing(int x, int y)
{
    if (isDrawing) currentShape.endPos.set(x, y);
}

ofVec3f Scene::calculProfondeur(const ofVec2f& pointEcran, float profondeur)
{
    ofVec3f pointProche = camera->screenToWorld({ pointEcran.x, pointEcran.y, camera->getNearClip() });
    ofVec3f pointLoin = camera->screenToWorld({ pointEcran.x, pointEcran.y, camera->getFarClip() });
    ofVec3f direction = (pointLoin - pointProche).getNormalized();
    float t = (profondeur - pointProche.z) / direction.z;
    return pointProche + direction * t;
}

void Scene::finalizeDrawing()
{
    if (!isDrawing) return;
    
    float profondeurCible = 0.0f;
    ofVec3f debutScene = calculProfondeur(currentShape.startPos, profondeurCible);
    ofVec3f findScene = calculProfondeur(currentShape.endPos, profondeurCible);
    ofVec3f centreScene = (debutScene + findScene) * 0.5f;
    
    PrimitiveObject* prim = new PrimitiveObject();
    prim->type = currentShape.type;
    prim->strokeColor = currentShape.strokeColor;
    prim->fillColor = currentShape.fillColor;
    prim->lineWidth = currentShape.lineWidth;
    
    prim->setPosition(centreScene);
    
    prim->positionInitiale = debutScene - centreScene;
    prim->positionFinale = findScene - centreScene;
    
    addObject(prim);
    isDrawing = false;
}

void Scene::drawShape(const Shape& shape)
{
    ofSetLineWidth(shape.lineWidth);
    
    if (shape.outline)
    {
        ofSetColor(shape.strokeColor);
        ofNoFill();
    }
    else
    {
        ofSetColor(shape.fillColor);
        ofFill();
    }
    
    switch (shape.type)
    {
        case PrimitiveType::POINT:
            ofSetColor(shape.fillColor);
            ofFill();
            ofDrawCircle(shape.startPos, shape.lineWidth);
            break;
        case PrimitiveType::LINE:
        {
            ofFill();
            ofSetColor(shape.strokeColor);
            glm::vec2 dir = glm::normalize(glm::vec2(shape.endPos - shape.startPos));
            glm::vec2 perp = glm::vec2(-dir.y, dir.x) * shape.lineWidth * 0.5f;
            ofBeginShape();
            ofVertex(shape.startPos + perp);
            ofVertex(shape.startPos - perp);
            ofVertex(shape.endPos - perp);
            ofVertex(shape.endPos + perp);
            ofEndShape(true);
        }
            break;
        case PrimitiveType::RECTANGLE:
            ofDrawRectangle(shape.startPos.x, shape.startPos.y,
                            shape.endPos.x - shape.startPos.x,
                            shape.endPos.y - shape.startPos.y);
            break;
        case PrimitiveType::CIRCLE:
        {
            float radius = shape.startPos.distance(shape.endPos);
            ofDrawCircle(shape.startPos, radius);
        }
            break;
        case PrimitiveType::ELLIPSE:
            ofDrawEllipse((shape.startPos.x + shape.endPos.x) / 2,
                          (shape.startPos.y + shape.endPos.y) / 2,
                          abs(shape.endPos.x - shape.startPos.x),
                          abs(shape.endPos.y - shape.startPos.y));
            break;
        case PrimitiveType::TRIANGLE:
        {
            glm::vec2 p1 = shape.startPos;
            glm::vec2 p2 = shape.endPos;
            glm::vec2 p3 = glm::vec2((p1.x + p2.x) / 2, p1.y - abs(p2.x - p1.x));
            ofDrawTriangle(p1, p2, p3);
        }
            break;
    }
}

Scene::~Scene()
{
    for (auto obj : objects)
    {
        delete obj;
    }
    std::free(locators);
        
}
