#include "Application.h"
#include <algorithm>

void Application::setup() {
	ofSetWindowTitle("Infographie");
	ofLog() << "Application démarre...";

	scene.setup();

	user_camera_movement.setup(scene);
}

void Application::update() {
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;

	user_camera_movement.update(time_elapsed);

	scene.update();
}

void Application::draw() {
	img.showImage();
	scene.draw();
}



void Application::windowResized(int w, int h) {
	ofLog() << "Fenêtre redimensionnée : " << w << "x" << h;
}

void Application::keyPressed(int key)
{
	switch (key)
	{
	case OF_KEY_LEFT:
		user_camera_movement.move_left = true;
		break;
	case OF_KEY_UP:
		user_camera_movement.move_forward = true;
		break;
	case OF_KEY_RIGHT:
		user_camera_movement.move_right = true;
		break;
	case OF_KEY_DOWN:
		user_camera_movement.move_backwards = true;
		break;

	case 'a':
		user_camera_movement.move_left = true;
		break;
	case 'w':
		user_camera_movement.move_forward = true;
		break;
	case 'd':
		user_camera_movement.move_right = true;
		break;
	case 's':
		user_camera_movement.move_backwards = true;
		break;
	case 'e':
		user_camera_movement.move_upwards = true;
		break;
	case 'q':
		user_camera_movement.move_downwards = true;
		break;

	default:
		break;
	}
}

void Application::keyReleased(int key)
{
	switch (key)
	{
	case OF_KEY_LEFT:
		user_camera_movement.move_left = false;
		break;
	case OF_KEY_UP:
		user_camera_movement.move_forward = false;
		break;
	case OF_KEY_RIGHT:
		user_camera_movement.move_right = false;
		break;
	case OF_KEY_DOWN:
		user_camera_movement.move_backwards = false;
		break;

	case 'a':
		user_camera_movement.move_left = false;
		break;
	case 'w':
		user_camera_movement.move_forward = false;
		break;
	case 'd':
		user_camera_movement.move_right = false;
		break;
	case 's':
		user_camera_movement.move_backwards = false;
		break;
	case 'e':
		user_camera_movement.move_upwards = false;
		break;
	case 'q':
		user_camera_movement.move_downwards = false;
		break;

	case ' ':
		img.imageExport("exportImage", "png");
		break;
	default:
		break;
	}
}

void Application::dragEvent(ofDragInfo dragInfo) {
	if (!dragInfo.files.empty()) {
		for (const auto& file : dragInfo.files) {
			img.importImage(file);
		}

	}
}


void Application::exit()
{
	ofLog() << "<app::exit>";
}
