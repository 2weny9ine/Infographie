#include "KeyPressedEventHandler.h"
#include "Application.h"

KeyPressedEventHandler::KeyPressedEventHandler() {}

void KeyPressedEventHandler::handleKeyPressed(int key)
{
    Application& app = Application::getInstance();
    User_Camera_Movement& userCameraMovement = app.getUserCameraMovement();

    switch (key)
    {
        case OF_KEY_LEFT:
        case 'a':
            userCameraMovement.move_left = true;
            break;
        case OF_KEY_UP:
        case 'w':
            userCameraMovement.move_forward = true;
            break;
        case OF_KEY_RIGHT:
        case 'd':
            userCameraMovement.move_right = true;
            break;
        case OF_KEY_DOWN:
        case 's':
            userCameraMovement.move_backwards = true;
            break;
        case 'e':
            userCameraMovement.move_upwards = true;
            break;
        case 'q':
            userCameraMovement.move_downwards = true;
            break;
        case 'r':
            if (app.getScene().cursor.getState() == CursorState::RESIZE)
                app.getScene().cursor.setState(CursorState::DEFAULT);
            else
                app.getScene().cursor.setState(CursorState::RESIZE);
            break;
        case 'm':
            if (app.getScene().cursor.getState() == CursorState::MOVE)
                app.getScene().cursor.setState(CursorState::DEFAULT);
            else
                app.getScene().cursor.setState(CursorState::MOVE);
            break;
        default:
            break;
    }
}
