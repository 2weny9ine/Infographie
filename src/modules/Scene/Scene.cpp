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
				object->position += delta;

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
				object->scale.x += mouseDelta.y * scaleFactor;
				object->scale.y += mouseDelta.y * scaleFactor;
				object->scale.z += mouseDelta.y * scaleFactor;
				object->scale.x = max(1.0f, object->scale.x);
				object->scale.y = max(1.0f, object->scale.y);
				object->scale.z = max(1.0f, object->scale.z);
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
				object->rotation.x += mouseDelta.y * rotationSpeed;
				object->rotation.y += mouseDelta.x * rotationSpeed;
				object->rotation.z += (mouseDelta.x + mouseDelta.y) * rotationSpeed * 0.5f;
			}


			gui->top_left->localTransformations[3] += mouseDelta.y * rotationSpeed;
			gui->top_left->localTransformations[4] -= mouseDelta.x * rotationSpeed;
			gui->top_left->localTransformations[5] += (mouseDelta.x + mouseDelta.y) * rotationSpeed * 0.5f;

			update_Attributes();
		}
		lastMouseScreen = mouseScreen;
	}
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

	for (auto obj : objects)
	{
		obj->draw();
	}

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
	update_Attributes();
}

void Scene::update_Attributes()
{
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
	if (selectedObjects.size() > 0)
	{
		//Set the locator to a middle point
		ofVec3f centroid(0, 0, 0);
		for (Object3D* object : selectedObjects)
		{
			centroid += object->position;
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

