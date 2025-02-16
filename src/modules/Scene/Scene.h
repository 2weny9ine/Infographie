#pragma once
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofMain.h"
#include <vector>

#include "objects/Object3D.h"

struct Locator
{
	float position[3];
	float rotation[3];
	float proportion[3];
	bool isSelected;
};

class Scene
{
public:
	Locator* locators;

	ofNode node;

	ofVec3f vector_position, vector_rotation, vector_proportion;

	ofCamera* camera;

	float center_x, center_y;
	float offset_x, offset_y, offset_z;
	float delta_x, delta_y, delta_z;

	float speed;

	int radius;

	int locator_count;
	int locator_buffer_head;

	int mouse_press_x, mouse_press_y;
	int mouse_current_x, mouse_current_y;

	bool is_active_translation, is_active_rotation, is_active_proportion;

	bool is_mouse_button_pressed;

	void dispatch_locators(int count, float range);

	void draw_locator(float scale);

	void resetSelection();

	void draw_zone(float x1, float y1, float x2, float y2) const;

	void selectAllInBounds(float x1, float y1, float x2, float y2);

	void setup(ofCamera* cam);
	void update();
	void draw();

	void addObject(Object3D* obj);
	void removeObject(Object3D* obj);

	~Scene();

private:
	std::vector<Object3D*> objects;
};
