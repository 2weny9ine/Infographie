#include "KeyEventHandler.h"
#include "Application.h"
#include <set>

KeyEventHandler::KeyEventHandler() : ctrlPressed(false), duplicated(false) {}

void KeyEventHandler::handleKeyPressed(int key)
{
    Application& app = Application::getInstance();
    User_Camera_Movement& userCameraMovement = app.getUserCameraMovement();

    // Add key to the set of pressed keys
    pressedKeys.insert(key);

    // Check for CTRL key press
    if (key == OF_KEY_CONTROL)
    {
        ctrlPressed = true;
    }

    // Handle other key presses
    if (!ctrlPressed)
    {
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
            case 'p':
                if (app.getScene().cursor.getState() == CursorState::RESIZE)
                    app.getScene().cursor.setState(CursorState::DEFAULT);
                else
                    app.getScene().cursor.setState(CursorState::RESIZE);
                break;
            case 't':
                if (app.getScene().cursor.getState() == CursorState::MOVE)
                    app.getScene().cursor.setState(CursorState::DEFAULT);
                else
                    app.getScene().cursor.setState(CursorState::MOVE);
                break;
            default:
                break;
        }
    }

    // Debug: Print all pressed keys
    for (int k : pressedKeys)
    {
        ofLogNotice() << k;
    }
}

void KeyEventHandler::handleKeyReleased(int key)
{
    Application& app = Application::getInstance();
    User_Camera_Movement& userCameraMovement = app.getUserCameraMovement();

    // Remove key from the set of pressed keys
    pressedKeys.erase(key);

    // Reset CTRL flag when released
    if (key == OF_KEY_CONTROL)
    {
        ctrlPressed = false;
    }

    // Reset duplicated flag when 4 is released
    if (key == 4)
    {
        duplicated = false;
    }

    // Handle other key releases

        switch (key)
        {
            case OF_KEY_BACKSPACE:
                app.getGui().top_right->deleteSelected();
                break;
            
            case OF_KEY_LEFT:
            case 'a':
                userCameraMovement.move_left = false;
                break;
            case OF_KEY_UP:
            case 'w':
                userCameraMovement.move_forward = false;
                break;
            case OF_KEY_RIGHT:
            case 'd':
                userCameraMovement.move_right = false;
                break;
            case OF_KEY_DOWN:
            case 's':
                userCameraMovement.move_backwards = false;
                break;
            case 'e':
                userCameraMovement.move_upwards = false;
                break;
            case 'q':
                userCameraMovement.move_downwards = false;
                break;
            case 'p':
            if (app.getScene().currentTransform != Scene::TransformMode::Resize)
            {
                app.getScene().currentTransform = Scene::TransformMode::Resize;
                app.getScene().cursor.setState(CursorState::RESIZE);
            }
            else
            {
                app.getScene().currentTransform = Scene::TransformMode::None;
                app.getScene().cursor.setState(CursorState::DEFAULT);
            }
            break;
            
        case 't':
            if (app.getScene().currentTransform != Scene::TransformMode::Translate)
            {
                app.getScene().currentTransform = Scene::TransformMode::Translate;
                app.getScene().cursor.setState(CursorState::MOVE);
            }
            else
            {
                app.getScene().currentTransform = Scene::TransformMode::None;
                app.getScene().cursor.setState(CursorState::DEFAULT);
            }
            break;
            
        case 'r':
            if (app.getScene().currentTransform != Scene::TransformMode::Rotate)
            {
                app.getScene().currentTransform = Scene::TransformMode::Rotate;
                app.getScene().cursor.setState(CursorState::ROTATE);
            }
            else
            {
                app.getScene().currentTransform = Scene::TransformMode::None;
                app.getScene().cursor.setState(CursorState::DEFAULT); 
            }
            break;
            default:
                break;
        }
}

void KeyEventHandler::update()
{
    Application& app = Application::getInstance();

    if (ctrlPressed && pressedKeys.count(4) && !duplicated)
    {
        app.getScene().duplicateSelectedInstances();
        duplicated = true; // Set flag to prevent multiple firings
        ofLogNotice() << "CTRL + D detected, duplicating instances";
    }
}
