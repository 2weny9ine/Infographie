#include "Scene.h"
#include "Image.h"
#include "ofMain.h"

void Scene::setup(ofCamera* cam)
{
	grid = new Grid();

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

	//allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des localisateurs
	locators = (Locator*)std::malloc(locator_count * sizeof(Locator));
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

	//ofTranslate(center_x + offset_x, center_y + offset_y, offset_z);

	//Boucle servant d'exemple pour extraire des valeurs de position, rotation et scale
	for (int index = 0; index < locator_buffer_head; ++index)
	{
		ofSetLineWidth(4);
		ofSetColor(127);
		ofFill();
		ofPushMatrix();
		//extraction des attributs de translation
		vector_position.x = locators[index].position[0];
		vector_position.y = locators[index].position[1];
		vector_position.z = locators[index].position[2];

		//assignation des attributs de translation
		node.setPosition(vector_position.x, vector_position.y, vector_position.z);

		vector_rotation.x = locators[index].rotation[0];
		vector_rotation.y = locators[index].rotation[1];
		vector_rotation.z = locators[index].rotation[2];

		node.setOrientation(vector_rotation);

		vector_proportion.x = locators[index].proportion[0];
		vector_proportion.y = locators[index].proportion[1];
		vector_proportion.z = locators[index].proportion[2];

		node.setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);

		node.draw();

		// Dessine une outline blanche si le node est sélectionné
		if (locators[index].isSelected) {
			ofSetColor(255);
			ofNoFill();
			ofSetLineWidth(6);
			node.draw();
		}

		ofPopMatrix();
	}

	//Dessine l'origine de la scène
	draw_locator(10.0f);
	for (auto obj : objects)
	{
		obj->draw();
	}

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

///Cette méthode a pour but de servir d'exemple pour effectuer des transformations sur des localisateurs
void Scene::dispatch_locators(int count, float range)
{
	float scale;

	//validation
	if (count <= 0 || range <= 0 || count > locator_count)
		return;

	//calculer la valeur de la moitié du diamètre du cube
	float halfRange = range / 2.0f;

	//configurer le nombre de localisateurs
	locator_buffer_head = count;

	for (int index = 0; index < locator_buffer_head; ++index)
	{
		// déterminer des valeurs de positions
		vector_position.x = ofRandom(-halfRange, halfRange);
		vector_position.y = ofRandom(-halfRange, halfRange);
		vector_position.z = ofRandom(-halfRange, halfRange);

		//déterminer une rotation aléatoire
		vector_rotation.x = 0;
		vector_rotation.y = ofRandom(0, 360);
		vector_rotation.z = 0;

		//déterminer une proportion uniforme au hasard entre 10% et 200%
		scale = ofRandom(0.1f, 2.0f);
		vector_proportion.x = scale;
		vector_proportion.y = -scale;
		vector_proportion.z = scale;

		//configurer les attributs de transformation du localisateur
		locators[index].position[0] = vector_position.x;
		locators[index].position[1] = vector_position.y;
		locators[index].position[2] = vector_position.z;

		locators[index].rotation[0] = vector_rotation.x;
		locators[index].rotation[1] = vector_rotation.y;
		locators[index].rotation[2] = vector_rotation.z;

		locators[index].proportion[0] = vector_proportion.x;
		locators[index].proportion[1] = vector_proportion.y;
		locators[index].proportion[2] = vector_proportion.z;
		//Reset du boolean isSelected
		locators[index].isSelected = false;
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
	float minX = min(x1, x2);
	float maxX = max(x1, x2);
	float minY = min(y1, y2);
	float maxY = max(y1, y2);

	for (int index = 0; index < locator_buffer_head; ++index)
	{
		glm::vec3 locatorPos = glm::vec3(locators[index].position[0], locators[index].position[1], locators[index].position[2]);
		glm::vec3 screenPos = camera->worldToScreen(locatorPos);

		if (screenPos.x >= minX && screenPos.x <= maxX && screenPos.y >= minY && screenPos.y <= maxY) {
			locators[index].isSelected = true;  // Select the node
		}
		else {
			locators[index].isSelected = false; // Deselect it
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
	for (int index = 0; index < locator_buffer_head; ++index)
	{
		locators[index].isSelected = false;//Toggle la selection
	}
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

