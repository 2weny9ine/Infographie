#include "Cursor.h"

Cursor::Cursor() : x(0), y(0), state(CursorState::DEFAULT), isMousePressed(false) {}

void Cursor::setup() {
    ofHideCursor;  
}

void Cursor::update(int x, int y, bool isPressed) {
    this->x = x;
    this->y = y;
    isMousePressed = isPressed;

    if (isPressed) state = CursorState::PRESSED;
}

void Cursor::draw() {
    ofPushStyle();
    switch (state) {
    case CursorState::DEFAULT:   drawDefault(); break;
    case CursorState::HOVER:     drawHover(); break;
    case CursorState::PRESSED:   drawPressed(); break;
    case CursorState::RESIZE:    drawResize(); break;
    case CursorState::MOVE:      drawMove(); break;
    case CursorState::DRAGGING:  drawDragging(); break;
    case CursorState::HIDDEN:    break; 
    }
    ofPopStyle();
}

void Cursor::setState(CursorState newState) {
    state = newState;
}

CursorState Cursor::getState() const {
    return state;
}


void Cursor::drawDefault() const {
}

void Cursor::drawHover() const {
    ofSetColor(0, 255, 0); 
    ofDrawTriangle(x, y - 10, x - 10, y + 10, x + 10, y + 10);
}

void Cursor::drawPressed() const {
    ofSetColor(255, 0, 0);  
    ofDrawRectangle(x - 5, y - 5, 10, 10);
}

void Cursor::drawResize() const {
    ofSetColor(255, 165, 0); 
    ofDrawEllipse(x, y, 15, 10);
}

void Cursor::drawMove() const {
    ofSetColor(255, 165, 0);
    ofDrawBitmapString("<->", x + 5, y + 5);
}

void Cursor::drawDragging() const {
    ofSetColor(0, 0, 255); 
    ofDrawCircle(x, y, 8);
}