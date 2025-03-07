#include "primitiveObject.h"

PrimitiveObject::PrimitiveObject()
: type(PrimitiveType::RECTANGLE)
, strokeColor(255)
, fillColor(127)
, lineWidth(2.0f)
{
    
    positionInitiale.set(0, 0);
    positionFinale.set(0, 0);
}

PrimitiveObject::~PrimitiveObject() {
    
}

void PrimitiveObject::setup() {
    
}

void PrimitiveObject::update(float dt) {
    
}

void PrimitiveObject::draw() {
    ofPushMatrix();
    {
        
        ofTranslate(position);
        ofRotateXDeg(rotation.x);
        ofRotateYDeg(rotation.y);
        ofRotateZDeg(rotation.z);
        ofScale(scale.x, scale.y, scale.z);
        
        ofSetLineWidth(lineWidth);
        
        
        switch(type) {
            case PrimitiveType::POINT:
                ofSetColor(strokeColor);
                ofDrawCircle(positionInitiale, lineWidth);
                break;
            case PrimitiveType::LINE:
                ofSetColor(strokeColor);
                ofDrawLine(positionInitiale, positionFinale);
                break;
            case PrimitiveType::RECTANGLE:
                if (fillColor.a > 0) {
                    ofSetColor(fillColor);
                    ofFill();
                    ofDrawRectangle(positionInitiale.x, positionInitiale.y,
                                    positionFinale.x - positionInitiale.x,
                                    positionFinale.y - positionInitiale.y);
                }
                ofNoFill();
                ofSetColor(strokeColor);
                ofDrawRectangle(positionInitiale.x, positionInitiale.y,
                                positionFinale.x - positionInitiale.x,
                                positionFinale.y - positionInitiale.y);
                break;
            case PrimitiveType::CIRCLE: {
                float rayon = positionInitiale.distance(positionFinale);
                if (fillColor.a > 0) {
                    ofSetColor(fillColor);
                    ofFill();
                    ofDrawCircle(positionInitiale, rayon);
                }
                ofNoFill();
                ofSetColor(strokeColor);
                ofDrawCircle(positionInitiale, rayon);
                break;
            }
            case PrimitiveType::ELLIPSE:
                if (fillColor.a > 0) {
                    ofSetColor(fillColor);
                    ofFill();
                    ofDrawEllipse((positionInitiale.x + positionFinale.x) / 2,
                                  (positionInitiale.y + positionFinale.y) / 2,
                                  abs(positionFinale.x - positionInitiale.x),
                                  abs(positionFinale.y - positionInitiale.y));
                }
                ofNoFill();
                ofSetColor(strokeColor);
                ofDrawEllipse((positionInitiale.x + positionFinale.x) / 2,
                              (positionInitiale.y + positionFinale.y) / 2,
                              abs(positionFinale.x - positionInitiale.x),
                              abs(positionFinale.y - positionInitiale.y));
                break;
            case PrimitiveType::TRIANGLE: {
                ofVec2f pointInitial = positionInitiale;
                ofVec2f pointFinal   = positionFinale;
                ofVec2f pointMilieu((positionInitiale.x + positionFinale.x) / 2,
                                    positionInitiale.y - abs(positionFinale.x - positionInitiale.x));
                if (fillColor.a > 0) {
                    ofSetColor(fillColor);
                    ofFill();
                    ofDrawTriangle(pointInitial, pointFinal, pointMilieu);
                }
                ofNoFill();
                ofSetColor(strokeColor);
                ofDrawTriangle(pointInitial, pointFinal, pointMilieu);
                break;
            }
        }
    }
    
    ofPopMatrix();
}
