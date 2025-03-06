#pragma once
#include "ofMain.h"

enum class CursorState {
    DEFAULT,
    HOVER,
    PRESSED,
    RESIZE,
    MOVE,
    HIDDEN,
    DRAGGING
};


class Cursor {
public:
    Cursor();

    void setup();
    void update(int x, int y, bool isPressed);
    void draw();

    void setState(CursorState newState);
    CursorState getState() const;

private:
    float x, y;
    CursorState state;
    bool isMousePressed;

    void drawDefault() const;
    void drawHover() const;
    void drawPressed() const;
    void drawResize() const;
    void drawMove() const;
    void drawDragging() const;
};
