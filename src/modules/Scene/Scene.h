#pragma once
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofMain.h"

//Structure d'un localisateur de transformation
struct Locator
{
	float position[3];
	float rotation[3];
	float proportion[3];
};

class Scene
{
public :
	Locator* locators;

	ofNode node;

	ofVec3f vector_position, vector_rotation, vector_proportion;

	float center_x, center_y;

	float offset_x, offset_z;

	float delta_x, delta_z;

	float speed;

	int locator_count;

	int locator_buffer_head;

	bool is_active_translation, is_active_rotation, is_active_proportion;

	void dispatch_locators(int count, float range);

	void draw_locator(float scale);

	void setup();
	void update();
	void draw();

	~Scene();

};

