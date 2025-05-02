// Topologie.cpp
#include "Topologie.h"
#include "Scene.h"
#include <cmath>

Topologie::Topologie(Scene* sc)
: scene(sc)
{}

Topologie::~Topologie(){
    for(auto* cp : ctrlPointObjs){
        scene->removeObject(cp);
        delete cp;
    }
}

void Topologie::setup(){}

void Topologie::setNumPoints(int n){
    n = ofClamp(n, minPoints, maxPoints);
    if(n == numPoints) return;
    numPoints = n;
    if(visible) regenerate();
}

void Topologie::setVisible(bool v){
    if(v == visible) return;
    visible = v;
    if(visible) regenerate();
    else {
        for(auto* cp : ctrlPointObjs){
            scene->removeObject(cp);
            delete cp;
        }
        ctrlPointObjs.clear();
        ctrlPoints.clear();
        curve.clear();
    }
}

void Topologie::regenerate(){
    for(auto* cp : ctrlPointObjs){
        scene->removeObject(cp);
        delete cp;
    }
    ctrlPointObjs.clear();
    ctrlPoints.clear();
    curve.clear();

    for(int i = 0; i < numPoints; ++i){
        float u = i / float(numPoints - 1);
        float x = 100 + 600 * u;
        float y = 300 + 100 * sin(u * TWO_PI);
        ctrlPoints.emplace_back(x, y, 0);
    }

    for(auto& p : ctrlPoints){
        Sphere* cp = new Sphere();
        cp->setScale(glm::vec3(0.2f));
        cp->setColor(ofColor::red);
        cp->setOpacity(1.0f);
        cp->setPosition(p);
        scene->addObject(cp);
        ctrlPointObjs.push_back(cp);
    }
}

void Topologie::update(float){
    if(!visible) return;
    curve.clear();
    for(size_t i = 0; i < ctrlPointObjs.size(); ++i){
        ctrlPoints[i] = ctrlPointObjs[i]->getPosition();
    }
    for(size_t i = 0; i + 3 < ctrlPoints.size(); i += 3){
        for(int j = 0; j <= resolution; ++j){
            float t = j / float(resolution);
            glm::vec3 out;
            bezierCubic(t,
                        ctrlPoints[i],
                        ctrlPoints[i+1],
                        ctrlPoints[i+2],
                        ctrlPoints[i+3],
                        out);
            curve.addVertex(out);
        }
    }
}

void Topologie::bezierCubic(float t,const glm::vec3& p0,const glm::vec3& p1,const glm::vec3& p2,
const glm::vec3& p3,glm::vec3& out){
    float u  = 1 - t;
    float tt = t * t;
    float uu = u * u;
    out = uu * u * p0
        + 3 * uu * t * p1
        + 3 * u * tt * p2
        + tt * t * p3;
}

void Topologie::draw(){
    if(!visible) return;
    ofPushStyle();
    ofSetLineWidth(2);
    ofSetColor(255, 255, 0);
    curve.draw();
    ofPopStyle();
}
