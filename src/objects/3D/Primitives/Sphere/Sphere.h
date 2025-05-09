#pragma once
#include "Object3D.h"
#include "ofMesh.h"
#include "modules/Properties/ModularProperties.h"
#include "modules/Texture/ImageFilterManager.h"


class Sphere : public Object3D {
public:
    Sphere();
    Sphere(const Sphere& instance);
    ~Sphere() override;

    void setup() override;
    void draw() override;
    void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;
    void drawBoundingBox() override;
    Sphere* copy() const override;

    virtual std::vector<Property> getProperties() const override;
    virtual void setProperty(const Property& prop) override;

    bool intersect(const Ray& ray, Intersection& intersection) override;

private:
    ofSpherePrimitive sphere;
    int resolution;
    ofImage textureImage;////texture
    bool hasTexture = false;////texture
    std::string textureName = "wood";//texture
    TextureFilterType currentFilter = TextureFilterType::None;
    ImageFilterManager filterManager;
    TextureFilterType lastAppliedFilter = TextureFilterType::None;

};
