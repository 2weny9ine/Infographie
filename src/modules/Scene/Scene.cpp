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
}

void Scene::update()
{
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;
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


	//Dessine l'origine de la scène
	draw_locator(10.0f);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : objects)
	{
		obj->draw();
	}

	glDisable(GL_DEPTH_TEST);

	ofPopMatrix();
	camera->end();
	// | ESPACE POUR DESSINER LES VECTEURS 2D |
	// V									  V

	//Dessine la zone de sélection
	if (is_mouse_button_pressed)
	{
		// dessiner la zone de sélection
		draw_zone(
			mouse_press_x,
			mouse_press_y,
			mouse_current_x,
			mouse_current_y);
	}
}

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

Scene::~Scene()
{
	for (auto obj : objects)
	{
		delete obj;
	}
	std::free(locators);
}

