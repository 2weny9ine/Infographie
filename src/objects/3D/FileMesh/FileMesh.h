#pragma once
#include "objects/Object3D.h"
#include "ofxAssimpModelLoader.h"

class FileMesh : public Object3D {
public:
    FileMesh();
    FileMesh(const FileMesh& instance);
    virtual ~FileMesh();

    bool loadModel(const std::string& path);
    std::string getModelPath() const;

    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    virtual ofRectangle getScreenBoundingBox(ofCamera* cam) override;
    virtual void getWorldBounds(glm::vec3& outMin, glm::vec3& outMax) const override;

    virtual void drawBoundingBox() override;

    virtual FileMesh* copy() const override;

private:
    ofxAssimpModelLoader model;
    std::string modelPath;
};
