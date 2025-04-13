#include "Sphere.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>

Sphere::Sphere()
{
	ofLogNotice("Sphere::Sphere") << "Sphere constructor called.";
	setup();
}

Sphere::Sphere(const Sphere& other)
	: Object3D(other), sphere(other.sphere), resolution(other.resolution)
{
	ofLogNotice("Sphere::Sphere(copy)") << "Sphere copy constructor called.";
	setup();
}

Sphere::~Sphere()
{
	ofLogNotice("Sphere::~Sphere") << "Sphere destructor called.";
}

void Sphere::setup()
{
	resolution = 16;
	sphere.set(5.0f, resolution);
	sphere.setPosition(0, 0, 0);
}

void Sphere::draw()
{
	ofPushMatrix();

	ofTranslate(position);
	ofRotateXDeg(rotation.x);
	ofRotateYDeg(rotation.y);
	ofRotateZDeg(rotation.z);
	ofScale(scale);

	ofSetColor(color, opacity * 255);
	sphere.draw();

	ofPopMatrix();
}

void Sphere::getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const
{
	float r = sphere.getRadius();
	glm::vec3 localMin(-r, -r, -r);
	glm::vec3 localMax(r, r, r);

	ofMatrix4x4 RS;
	RS.makeIdentityMatrix();
	RS.rotate(rotation.x, 1, 0, 0);
	RS.rotate(rotation.y, 0, 1, 0);
	RS.rotate(rotation.z, 0, 0, 1);
	RS.scale(scale.x, scale.y, scale.z);

	glm::vec3 corners[8] = {
		{ localMin.x, localMin.y, localMin.z },
		{ localMax.x, localMin.y, localMin.z },
		{ localMin.x, localMax.y, localMin.z },
		{ localMax.x, localMax.y, localMin.z },
		{ localMin.x, localMin.y, localMax.z },
		{ localMax.x, localMin.y, localMax.z },
		{ localMin.x, localMax.y, localMax.z },
		{ localMax.x, localMax.y, localMax.z }
	};

	glm::vec3 actualMin(FLT_MAX, FLT_MAX, FLT_MAX);
	glm::vec3 actualMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	const float* m = RS.getPtr();
	for (auto& c : corners)
	{
		float x = c.x * m[0] + c.y * m[4] + c.z * m[8];
		float y = c.x * m[1] + c.y * m[5] + c.z * m[9];
		float z = c.x * m[2] + c.y * m[6] + c.z * m[10];
		glm::vec3 transformed(x, y, z);
		glm::vec3 worldCorner = transformed + position;
		actualMin = glm::min(actualMin, worldCorner);
		actualMax = glm::max(actualMax, worldCorner);
	}
	outMin = actualMin;
	outMax = actualMax;
}

void Sphere::drawBoundingBox()
{
	glm::vec3 bbMin, bbMax;
	getWorldBounds(bbMin, bbMax);
	glm::vec3 center = (bbMin + bbMax) * 0.5f;
	float w = bbMax.x - bbMin.x;
	float h = bbMax.y - bbMin.y;
	float d = bbMax.z - bbMin.z;
	ofPushStyle();
	ofNoFill();
	ofSetColor(strokeColor);
	ofSetLineWidth(lineWidth);
	ofDrawBox(center, w, h, d);
	ofPopStyle();
}

Sphere* Sphere::copy() const
{
	return new Sphere(*this);
}

std::vector<Property> Sphere::getProperties() const
{
	std::vector<Property> props = Object3D::getProperties();

	std::vector<Property> sphereProps = {
		{"resolution",PropertyType::Int,PropertyValue{resolution},1.0f, 64.0f, 1.0f, 0},
		{"surface type", PropertyType::SurfaceType, PropertyValue{static_cast<int>(surfaceType)} }
	};


	props.insert(props.end(), sphereProps.begin(), sphereProps.end());
	return props;
}


void Sphere::setProperty(const Property& prop)
{
	if (prop.name == "resolution")
	{
		resolution = std::get<int>(prop.value);
		sphere.set(sphere.getRadius(), resolution);
	}
	else
	{
		Object3D::setProperty(prop);
	}
}
