#include "KeyReleasedEventHandler.h"
#include "Application.h"

KeyReleasedEventHandler::KeyReleasedEventHandler() {}

void KeyReleasedEventHandler::handleKeyReleased(int key)
{
    Application& app = Application::getInstance();
    User_Camera_Movement& userCameraMovement = app.getUserCameraMovement();

    switch (key)
    {
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
                app.getScene().currentTransform = Scene::TransformMode::Resize;
            else
                app.getScene().currentTransform = Scene::TransformMode::None;
            break;
        case 't':
            if (app.getScene().currentTransform != Scene::TransformMode::Translate)
                app.getScene().currentTransform = Scene::TransformMode::Translate;
            else
                app.getScene().currentTransform = Scene::TransformMode::None;
            break;
        case 'r':
            if (app.getScene().currentTransform != Scene::TransformMode::Rotate)
                app.getScene().currentTransform = Scene::TransformMode::Rotate;
            else
                app.getScene().currentTransform = Scene::TransformMode::None;
            break;
        case ' ':
            app.getScene().img->imageExport("exportImage", "png");
            app.getScene().img->setExportTriggered(true);
            break;
        default:
            break;
    }
}
