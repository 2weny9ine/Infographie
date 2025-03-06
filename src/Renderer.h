//
//  Renderer.h
//  Infographie
//
//  Created by Joy Patrick on 2025-01-31.
//
#pragma once
#include <ofAppRunner.h>
#include <ofGraphics.h>
#include "ofMain.h"


class Renderer {
public:

	void setup();
	void update();
	void draw();
	void reset();
	ofColor backgroundColor; // Task 2.2: Added to manage background color
};

