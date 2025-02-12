#include "Scene.h"
#include "ofMain.h"

void Scene::setup()
{
	locator_count = 100;

	speed = 100.0f;

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

	//dispatch_locators(locator_count, std::min(ofGetWidth(), ofGetHeight()));
}

void Scene::update()
{
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;
}

void Scene::draw()
{
	ofPushMatrix();

	ofTranslate(center_x + offset_x, center_y + offset_y, offset_z);
	//Boucle servant d'exemple pour extraire des valeurs de position, rotation et scale
	for (int index = 0; index < locator_buffer_head; ++index)
	{
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

		ofPopMatrix();
	}

	//Dessine l'origine de la scène
	draw_locator(10.0f);

	ofPopMatrix();

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
	}
}

void Scene::draw_locator(float scale)
{
	ofSetLineWidth(4);
	ofSetColor(127);
	ofFill();
	ofPushMatrix();
	ofScale(scale, -scale, scale);
	node.setPosition(0.0f, 0.0f, 0.0f);
	node.draw();
	ofPopMatrix();
}

Scene::~Scene()
{
	std::free(locators);
}

