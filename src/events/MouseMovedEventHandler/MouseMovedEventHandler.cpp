#include "MouseMovedEventHandler.h"
#include "Application.h"

MouseMovedEventHandler::MouseMovedEventHandler() {}

void MouseMovedEventHandler::handleMouseMoved(int x, int y)
{
    Application& app = Application::getInstance();
    app.getScene().mouse_current_x = x;
    app.getScene().mouse_current_y = y;

    if (!app.getScene().is_mouse_button_pressed &&
        app.getScene().cursor.getState() != CursorState::MOVE &&
        app.getScene().cursor.getState() != CursorState::RESIZE)
    {
        if (app.getScene().isMouseOverObject(x, y))
        {
            app.getScene().cursor.setState(CursorState::HOVER);
        }
        else
        {
            app.getScene().cursor.setState(CursorState::DEFAULT);
        }
    }
}
