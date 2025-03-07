#pragma once
#include "Object3D.h"
#include "Top_Right_GUI.h"


class PrimitiveObject : public Object3D {
public:
    PrimitiveObject();
    virtual ~PrimitiveObject();
    
    
    virtual void setup() override;
    virtual void update(float dt) override;
    virtual void draw() override;
    
    
    PrimitiveType type;
    ofColor strokeColor;
    ofColor fillColor;
    float lineWidth;
    
    
    ofVec2f positionInitiale;
    ofVec2f positionFinale;
};
