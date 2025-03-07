#include "Scene.h"
#include "Image.h"
#include "ofMain.h"
#include "GUI.h"
#include "primitiveObject.h"

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


	//yacine
	/**************************************************************************/
	cursor.setup();
	/**************************************************************************/
}

void Scene::update()
{
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;

	ofVec3f mouseScreen(ofGetMouseX(), ofGetMouseY(), 0);

	if (currentTransform == Scene::TransformMode::Translate)
	{
		ofVec3f worldNear = camera->screenToWorld(ofVec3f(mouseScreen.x, mouseScreen.y, camera->getNearClip())); // Near plane
		ofVec3f worldFar = camera->screenToWorld(ofVec3f(mouseScreen.x, mouseScreen.y, camera->getFarClip()));  // Far plane

		ofVec3f direction = (worldFar - worldNear).getNormalized();
		float t = (planeZ - worldNear.z) / direction.z;

		ofVec3f mouseWorld = worldNear + direction * t;

		if (is_mouse_button_pressed)
		{
			ofVec3f delta = mouseWorld - lastMouseWorld;

			for (Object3D* object : selectedObjects)
				object->transformPosition(delta);

			gui->top_left->localTransformations[0] += delta.x;
			gui->top_left->localTransformations[1] += delta.y;
			gui->top_left->localTransformations[2] += delta.z;

			update_Attributes();
		}

		lastMouseWorld = mouseWorld;
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


			gui->top_left->localTransformations[6] += mouseDelta.y * scaleFactor;
			gui->top_left->localTransformations[7] += mouseDelta.y * scaleFactor;
			gui->top_left->localTransformations[8] += mouseDelta.y * scaleFactor;

			update_Attributes();
		}

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


			gui->top_left->localTransformations[3] += mouseDelta.y * rotationSpeed;
			gui->top_left->localTransformations[4] -= mouseDelta.x * rotationSpeed;
			gui->top_left->localTransformations[5] += (mouseDelta.x + mouseDelta.y) * rotationSpeed * 0.5f;

			update_Attributes();
		}
		lastMouseScreen = mouseScreen;
	}
	//yacine
	/**************************************************************************/
	cursor.update(mouse_current_x, mouse_current_y, is_mouse_button_pressed);
	/**************************************************************************/
}

Scene::Scene() {
    img = new Image(*this); 
	currentTransform = TransformMode::None;
	scaleFactor = 0.2f;
	rotationSpeed = 0.5f;
	planeZ = 0.0f;
}



void Scene::draw()
{
	//ESPACE POUR DESSINER LE 3D
	camera->begin();
	ofPushMatrix();
	
	grid->draw();


	//Dessine l'origine de la scène
	glEnable(GL_DEPTH_TEST);

	for (auto obj : objects)
	{
		obj->draw();
	}
	glDisable(GL_DEPTH_TEST);
	draw_locator(10.0f);
	ofPopMatrix();
	camera->end();
	// | ESPACE POUR DESSINER LES VECTEURS 2D |
	// V									  V

	//Dessine la zone de sélection
	if (is_mouse_button_pressed && currentTransform == TransformMode::None)
	{
		// dessiner la zone de sélection
		draw_zone(
			mouse_press_x,
			mouse_press_y,
			mouse_current_x,
			mouse_current_y);
	}


	//2.3
	/**************************************************************************/
	/**************************************************************************/
	for (auto& shape : shapes) drawShape(shape);
	if (isDrawing) drawShape(currentShape);
	//cursor.draw();
	/**************************************************************************/
	/**************************************************************************/
	
}

//2.2
/**************************************************************************/
/**************************************************************************/
void Scene::drawCursor() {
	glPushAttrib(GL_ALL_ATTRIB_BITS); // Save all OpenGL states
	ofDisableDepthTest();             // Disable depth to draw on top
	cursor.draw();                    // Draw custom cursor
	glPopAttrib();
}

bool Scene::isMouseOverObject(int mouseX, int mouseY) {
	for (auto obj : objects) {
		ofRectangle bbox = obj->getScreenBoundingBox(camera);
		if (bbox.inside(mouseX, mouseY)) {
			return true;
		}
	}
	return false;
}
/**************************************************************************/
/**************************************************************************/


void Scene::draw_zone(float x1, float y1, float x2, float y2) const
{
	float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
	float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());
	//Dessine la boite de sélection
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

	for (Object3D *object : objects)
	{
		ofVec3f objectPos = object->getPosition();
		glm::vec3 screenPos = camera->worldToScreen(objectPos);

		if (screenPos.x >= minX && screenPos.x <= maxX && screenPos.y >= minY && screenPos.y <= maxY) {
			object->setSelected(true);
			selectedObjects.push_back(object);
		}
		else {
			object->setSelected(false);
		}
	}
	update_Attributes();
}

void Scene::update_Attributes()
{
	if (selectedObjects.size() == 1)
	{
		Object3D* currentObject = selectedObjects[0];
		ofVec3f position = currentObject->getPosition();
		ofVec3f rotation = currentObject->getRotation();
		ofVec3f scale = currentObject->getScale();

		gui->top_left.get()->inputs[0]->setText(std::to_string(position.x));
		gui->top_left.get()->inputs[1]->setText(std::to_string(position.y));
		gui->top_left.get()->inputs[2]->setText(std::to_string(position.z));

		gui->top_left.get()->inputs[3]->setText(std::to_string(rotation.x));
		gui->top_left.get()->inputs[4]->setText(std::to_string(rotation.y));
		gui->top_left.get()->inputs[5]->setText(std::to_string(rotation.z));

		gui->top_left.get()->inputs[6]->setText(std::to_string(scale.x));
		gui->top_left.get()->inputs[7]->setText(std::to_string(scale.y));
		gui->top_left.get()->inputs[8]->setText(std::to_string(scale.z));
	}
	else if(gui->top_left->localTransformations == std::vector<float>{ 0,0,0,0,0,0,0,0,0 })
	{
		gui->top_left->resetTransformations();
	}
	else
	{
		gui->top_left.get()->inputs[0]->setText(std::to_string(gui->top_left->localTransformations[0]));
		gui->top_left.get()->inputs[1]->setText(std::to_string(gui->top_left->localTransformations[1]));
		gui->top_left.get()->inputs[2]->setText(std::to_string(gui->top_left->localTransformations[2]));

		gui->top_left.get()->inputs[3]->setText(std::to_string(gui->top_left->localTransformations[3]));
		gui->top_left.get()->inputs[4]->setText(std::to_string(gui->top_left->localTransformations[4]));
		gui->top_left.get()->inputs[5]->setText(std::to_string(gui->top_left->localTransformations[5]));

		gui->top_left.get()->inputs[6]->setText(std::to_string(gui->top_left->localTransformations[6]));
		gui->top_left.get()->inputs[7]->setText(std::to_string(gui->top_left->localTransformations[7]));
		gui->top_left.get()->inputs[8]->setText(std::to_string(gui->top_left->localTransformations[8]));
	}

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

			object->transformPosition(rotation);

			object->transformScale(scale);
		}
	}
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
}

void Scene::draw_locator(float scale)
{
	ofSetLineWidth(4);
	ofSetColor(127);
	ofFill();
	ofPushMatrix();
	if (selectedObjects.size() > 0)
	{
		//Set the locator to a middle point
		ofVec3f centroid(0, 0, 0);
		for (Object3D* object : selectedObjects)
		{
			centroid += object->getPosition();
		}
		node.setPosition(centroid / selectedObjects.size());
	}
	else
		node.setPosition(0,0,0);

	node.setOrientation(ofVec3f(0,0,0));
	node.setScale(scale);
	node.draw();
	ofPopMatrix();
}

//void Scene::draw_locator(float scale)
//{
//    ofSetLineWidth(4);
//    ofSetColor(127);
//    ofFill();
//    ofPushMatrix();
//
// 
//    node.setPosition(0, 0, 0);
//    node.setOrientation(ofVec3f(0, 0, 0));
//    node.setScale(scale);
//    node.draw();
//
//    ofPopMatrix();
//}


void Scene::resetSelection()
{
	for (Object3D* object : selectedObjects)
		object->setSelected(false);

	selectedObjects.clear();
	gui->top_left->localTransformations = { 0,0,0,0,0,0,0,0,0 };
}

void Scene::addObject(Object3D* obj) {
	objects.push_back(obj);
	ofLogNotice("Scene::addObject") << "Object added to scene.";
}

void Scene::removeObject(Object3D* obj)
{
	auto it = std::find(objects.begin(), objects.end(), obj);
	if (it != objects.end())
	{
		objects.erase(it);
	}
}

//2.2
/**************************************************************************/
/**************************************************************************/
void Scene::setOutlineEnabled(bool enabled) {
	outlineEnabled = enabled;
}
void Scene::updateDrawingProperties(const ofColor& stroke, const ofColor& fill, float width, bool outline) {
	currentShape.strokeColor = stroke;
	currentShape.fillColor = fill;
	currentShape.lineWidth = width;
	currentShape.outline = outline;
}

void Scene::setPrimitiveType(PrimitiveType primitive) {
	activePrimitive = primitive;
}

void Scene::startDrawing(int x, int y) {
	isDrawing = true;
	currentShape.type = activePrimitive;
	currentShape.startPos.set(x, y);
	currentShape.endPos.set(x, y);
	currentShape.outline = outlineEnabled;
}

void Scene::updateCurrentDrawing(int x, int y) {
	if (isDrawing) currentShape.endPos.set(x, y);
}


ofVec3f Scene::calculProfondeur(const ofVec2f &pointEcran, float profondeur) {
    ofVec3f pointProche = camera->screenToWorld({ pointEcran.x, pointEcran.y, camera->getNearClip() });
    ofVec3f pointLoin   = camera->screenToWorld({ pointEcran.x, pointEcran.y, camera->getFarClip() });
    ofVec3f direction   = (pointLoin - pointProche).getNormalized();
    float t = (profondeur - pointProche.z) / direction.z;
    return pointProche + direction * t;
}



void Scene::finalizeDrawing() {
    if (!isDrawing) return;
    
    float profondeurCible = 0.0f;

    ofVec3f debutScene = calculProfondeur(currentShape.startPos, profondeurCible);
    ofVec3f findScene   = calculProfondeur(currentShape.endPos, profondeurCible);
    

    ofVec3f centreScene = (debutScene + findScene) * 0.5f;
    

    PrimitiveObject* prim = new PrimitiveObject();
    prim->type = currentShape.type;
    prim->strokeColor = currentShape.strokeColor;
    prim->fillColor   = currentShape.fillColor;
    prim->lineWidth   = currentShape.lineWidth;
    

    prim->setPosition(centreScene);
    

    prim->positionInitiale = debutScene - centreScene;
    prim->positionFinale   = findScene - centreScene;
    
    addObject(prim);
    isDrawing = false;
}





void Scene::drawShape(const Shape& shape) {
	ofSetLineWidth(shape.lineWidth);

	if (shape.outline) { 
		ofSetColor(shape.strokeColor);
		ofNoFill();
	}
	else {
		ofSetColor(shape.fillColor);
		ofFill();
	}

	switch (shape.type) {
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
		ofDrawRectangle(shape.startPos.x, shape.startPos.y, shape.endPos.x - shape.startPos.x, shape.endPos.y - shape.startPos.y);
		break;
	case PrimitiveType::CIRCLE: {
		float radius = shape.startPos.distance(shape.endPos);
		ofDrawCircle(shape.startPos, radius);
		break;
	}
	case PrimitiveType::ELLIPSE:
		ofDrawEllipse((shape.startPos.x + shape.endPos.x) / 2, (shape.startPos.y + shape.endPos.y) / 2,
			abs(shape.endPos.x - shape.startPos.x), abs(shape.endPos.y - shape.startPos.y));
		break;
	case PrimitiveType::TRIANGLE:{ 
		glm::vec2 p1 = shape.startPos;
		glm::vec2 p2 = shape.endPos;
		glm::vec2 p3 = glm::vec2((p1.x + p2.x) / 2, p1.y - abs(p2.x - p1.x)); 

		ofDrawTriangle(p1, p2, p3);
		break;
	}
	}
}
/**************************************************************************/
/**************************************************************************/

Scene::~Scene()
{
	for (auto obj : objects)
	{
		delete obj;
	}
	std::free(locators);
}

