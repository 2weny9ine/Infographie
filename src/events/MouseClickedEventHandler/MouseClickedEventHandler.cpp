#include "MouseClickedEventHandler.h"
#include "Application.h"

MouseClickedEventHandler::MouseClickedEventHandler() {}

void MouseClickedEventHandler::handleMousePressed(int x, int y, int button)
{
    Application& app = Application::getInstance();
    app.getScene().is_mouse_button_pressed = true;
    app.getScene().mouse_current_x = x;
    app.getScene().mouse_current_y = y;
    app.getScene().mouse_press_x = x;
    app.getScene().mouse_press_y = y;
    app.getScene().cursor.setState(CursorState::PRESSED);

    if (app.getScene().isDrawingMode)
    {
        app.getScene().startDrawing(x, y);
    }
}

void MouseClickedEventHandler::handleMouseReleased(int x, int y, int button)
{
    Application& app = Application::getInstance();
    app.getScene().is_mouse_button_pressed = false;
    app.getScene().mouse_current_x = x;
    app.getScene().mouse_current_y = y;

    if (app.getScene().isMouseOverObject(x, y))
    {
        app.getScene().cursor.setState(CursorState::HOVER);
    }
    else
    {
        app.getScene().cursor.setState(CursorState::DEFAULT);
    }

    float distance = sqrt(pow(x - app.getScene().mouse_press_x, 2) + pow(y - app.getScene().mouse_press_y, 2));

    if (distance > 20.0f && app.getScene().currentTransform == Scene::TransformMode::None)
    {
        app.getScene().selectAllInBounds(app.getScene().mouse_press_x, app.getScene().mouse_press_y, x, y);
    }

    if (app.getScene().isDrawingMode)
    {
        app.getScene().finalizeDrawing();
    }
}
