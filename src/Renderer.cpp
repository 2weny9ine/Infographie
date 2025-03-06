//
//  Renderer.cpp
//  Infographie
//

#include "Renderer.h"
#include "Scene.h"

void Renderer::setup()
{
	ofSetFrameRate(60);
	ofSetBackgroundColor(31);
	ofEnableDepthTest();

	reset();
}
void Renderer:: draw() {
}
void Renderer::update()
{

}


void Renderer::reset()
{
	ofLog() << "<reset>";
}

