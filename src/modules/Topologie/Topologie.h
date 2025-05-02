#pragma once
#include "ofMain.h"
#include "objects/Object3D.h"
#include "Sphere.h"
#include <vector>

class Scene;

class Topologie : public Object3D {
public:
    Topologie(Scene* scene);
    ~Topologie() override;

    void setup() override;
    void update(float dt) override;
    void draw() override;

    void setVisible(bool v);
    void setNumPoints(int n);
    int getNumPoints() const { return numPoints; }

    static constexpr int minPoints = 4;
    static constexpr int maxPoints = 16;

private:
    void regenerate();
    void bezierCubic(
        float t,
        const glm::vec3& p0,
        const glm::vec3& p1,
        const glm::vec3& p2,
        const glm::vec3& p3,
        glm::vec3& out
    );

    Scene* scene;
    std::vector<glm::vec3> ctrlPoints;
    std::vector<Sphere*> ctrlPointObjs;
    ofPolyline curve;
    int numPoints = maxPoints;
    bool visible = false;
    int resolution = 50;
};
