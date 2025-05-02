#include "Sphere.h"
#include "modules/Configuration/Configuration.h"
#include "ofLog.h"
#include "Application.h"
#include <algorithm>
#include <sstream>
#include <glm/gtx/quaternion.hpp>
#include "modules/Texture/TextureManager.h"


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
    //texture
    ofTexture& tex = Application::getInstance().getTextureManager().getTexture(textureName);
    sphere.mapTexCoordsFromTexture(tex);
    hasTexture = true;
    filterManager.setup(&tex);
    //texture

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
    // texture
    if (hasTexture) {
        if (currentFilter != lastAppliedFilter) {
            ofLogNotice("[Filter]") << "Applying: " << static_cast<int>(currentFilter);
            filterManager.applyFilter(currentFilter);
            lastAppliedFilter = currentFilter;
        }

        ofTexture& tex = filterManager.getFilteredTexture();
        tex.bind();
        sphere.draw();
        tex.unbind();
    }
    else {
        sphere.draw();
    }
    //texture
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
    Property resProp;
    resProp.name = "resolution";
    resProp.type = PropertyType::Int;
    resProp.value = resolution;
    resProp.min = 1.0f;
    resProp.max = 64.0f;
    resProp.step = 1.0f;
    resProp.decimals = 0;
    props.push_back(resProp);

    // Texture toggle
    Property textureProp;
    textureProp.name = "Texture";
    textureProp.type = PropertyType::Bool;
    textureProp.value = hasTexture;
    props.push_back(textureProp);

    //texture choice
    Property texChoiceProp;
    texChoiceProp.name = "TextureType";
    texChoiceProp.type = PropertyType::String;
    texChoiceProp.value = textureName;
    texChoiceProp.options = { "wood", "tree", "brick", "checkerboard" };
    props.push_back(texChoiceProp);


    //filter
    Property filterProp;
    filterProp.name = "Filter";
    filterProp.type = PropertyType::String;
    switch (currentFilter) {
    case TextureFilterType::Emboss: filterProp.value = "Emboss"; break;
    case TextureFilterType::Sharpen: filterProp.value = "Sharpen"; break;
    case TextureFilterType::EdgeDetect: filterProp.value = "EdgeDetect"; break;
    default: filterProp.value = "None"; break;
    }
    filterProp.options = { "None", "Emboss", "Sharpen", "EdgeDetect" };
    props.push_back(filterProp);



    return props;
}


void Sphere::setProperty(const Property& prop)
{
    if (prop.name == "resolution")
    {
        resolution = std::get<int>(prop.value);
        sphere.set(sphere.getRadius(), resolution);
    }
    else if (prop.name == "Texture")
    {
        hasTexture = std::get<bool>(prop.value);
    }
    else if (prop.name == "TextureType") {
        textureName = std::get<std::string>(prop.value);

        ofTexture& newTex = Application::getInstance().getTextureManager().getTexture(textureName);

        sphere.mapTexCoordsFromTexture(newTex);
        filterManager.setup(&newTex);

        lastAppliedFilter = TextureFilterType::None;
    }
    else if (prop.name == "Filter") {
    std::string val = std::get<std::string>(prop.value);
    if (val == "Emboss") currentFilter = TextureFilterType::Emboss;
    else if (val == "Sharpen") currentFilter = TextureFilterType::Sharpen;
    else if (val == "EdgeDetect") currentFilter = TextureFilterType::EdgeDetect;
    else currentFilter = TextureFilterType::None;
}

    else
    {
        Object3D::setProperty(prop);
    }
}

bool Sphere::intersect(const Ray& ray, Intersection& intersection)
{
	glm::vec3 C = position;
	float    R = sphere.getRadius() * scale.x;

	glm::vec3 L = C - ray.origin;
	float tca = glm::dot(L, ray.direction);
	if (tca < 0.0f) return false;

	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > R * R)  return false;

	float thc = sqrt(R * R - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;
	float t = (t0 >= 0.0f) ? t0 : t1;
	if (t < 0.0f) return false;

	intersection.hit = true;
	intersection.distance = t;
	intersection.point = ray.origin + ray.direction * t;

	glm::vec3 N = (intersection.point - C) / R;
	intersection.normal = glm::normalize(N);
	intersection.object = this;
	return true;
}
