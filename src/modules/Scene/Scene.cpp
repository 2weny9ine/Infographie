#include "Scene.h"
#include "Image.h"
#include "ofMain.h"
#include "GUI.h"

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

	//yacine
	/**************************************************************************/
	cursor.update(mouse_current_x, mouse_current_y, is_mouse_button_pressed);
	/**************************************************************************/
}

Scene::Scene() {
    img = new Image(*this);  
}



void Scene::draw()
{
	//ESPACE POUR DESSINER LE 3D
	camera->begin();
	ofPushMatrix();
	
	grid->draw();


	//Dessine l'origine de la sc�ne
	draw_locator(10.0f);
	for (auto obj : objects)
	{
		obj->draw();
	}

	ofPopMatrix();
	camera->end();
	// | ESPACE POUR DESSINER LES VECTEURS 2D |
	// V									  V

	//Dessine la zone de s�lection
	if (is_mouse_button_pressed)
	{
		// dessiner la zone de s�lection
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
	//Dessine la boite de s�lection
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
		ofVec3f objectPos = object->position;
		glm::vec3 screenPos = camera->worldToScreen(objectPos);

		if (screenPos.x >= minX && screenPos.x <= maxX && screenPos.y >= minY && screenPos.y <= maxY) {
			object->selected = true;
			selectedObjects.push_back(object);
		}
		else {
			object->selected = false;
		}
	}

	if (selectedObjects.size() == 1)
	{
		Object3D* currentObject = selectedObjects[0];
		gui->top_left.get()->inputs[0]->setText(std::to_string(currentObject->position.x));
		gui->top_left.get()->inputs[1]->setText(std::to_string(currentObject->position.y));
		gui->top_left.get()->inputs[2]->setText(std::to_string(currentObject->position.z));

		gui->top_left.get()->inputs[3]->setText(std::to_string(currentObject->rotation.x));
		gui->top_left.get()->inputs[4]->setText(std::to_string(currentObject->rotation.y));
		gui->top_left.get()->inputs[5]->setText(std::to_string(currentObject->rotation.z));

		gui->top_left.get()->inputs[6]->setText(std::to_string(currentObject->scale.x));
		gui->top_left.get()->inputs[7]->setText(std::to_string(currentObject->scale.y));
		gui->top_left.get()->inputs[8]->setText(std::to_string(currentObject->scale.z));
	}
	else
	{
		gui->top_left->resetTransformations();
	}
}

void Scene::apply_Transformations(ofVec3f position, ofVec3f rotation, ofVec3f scale)
{
	if (selectedObjects.size() == 1)
	{
		selectedObjects[0]->position = position;
		selectedObjects[0]->rotation = rotation;
		selectedObjects[0]->scale = scale;
	}
	else if (selectedObjects.size() > 1)
	{
		for (Object3D* object : selectedObjects)
		{
			object->position += position;
			object->rotation += rotation;
			object->scale += scale;
		}
	}
}

void Scene::draw_locator(float scale)
{
	ofSetLineWidth(4);
	ofSetColor(127);
	ofFill();
	ofPushMatrix();
	node.setPosition(0.0f, 0.0f, 0.0f);
	node.setOrientation(ofVec3f(0,0,0));
	node.setScale(ofVec3f(10,-10,10));
	node.draw();
	ofPopMatrix();
}

void Scene::resetSelection()
{
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

void Scene::finalizeDrawing() {
	if (isDrawing) {
		shapes.push_back(currentShape);
		isDrawing = false;
	}
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

