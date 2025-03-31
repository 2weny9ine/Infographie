#include "MouseEventHandler.h"
#include "Application.h"

MouseEventHandler::MouseEventHandler()
    : isRightMouseButtonDown(false)
{}

bool MouseEventHandler::isMouseOverAnyGui(int x, int y) const
{
    Application& app = Application::getInstance();
    return app.getGui().isMouseOverGui(x, y);
}

void MouseEventHandler::handleMousePressed(int x, int y, int button)
{
    Application& app = Application::getInstance();
    Scene& scene = app.getScene();

    bool mouseOverGui = app.getGui().isMouseOverGui(x, y);

    if (button == 0) // Left mouse button pressed
    {
        scene.is_mouse_button_pressed = true;
        scene.mouse_current_x = x;
        scene.mouse_current_y = y;
        scene.mouse_press_x = x;
        scene.mouse_press_y = y;
        scene.cursor.setState(CursorState::PRESSED);

        if (scene.currentTransform == Scene::TransformMode::None)
        {
            if (!mouseOverGui)
            {
                scene.resetSelection();
            }
        }

        if (scene.isDrawingMode && !mouseOverGui)
        {
            scene.startDrawing(x, y);
        }
    }
    else if (button == 2)
    {
        isRightMouseButtonDown = true;
        scene.lastMouseScreen.set(x, y);
    }
}


void MouseEventHandler::handleMouseReleased(int x, int y, int button)
{
    Application& app = Application::getInstance();
    Scene& scene = app.getScene();

    scene.is_mouse_button_pressed = false;
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;

    bool mouseOverGui = isMouseOverAnyGui(x, y);

    if (scene.isMouseOverObject(x, y))
    {
        scene.cursor.setState(CursorState::HOVER);
    }
    else
    {
        scene.cursor.setState(CursorState::DEFAULT);
    }

    if (button == OF_MOUSE_BUTTON_LEFT && !mouseOverGui)
    {
        float distance = glm::distance(glm::vec2(x, y), glm::vec2(scene.mouse_press_x, scene.mouse_press_y));
        if (distance > 5.0f && scene.currentTransform == Scene::TransformMode::None && !scene.isDrawingMode)
        {
            scene.selectAllInBounds(scene.mouse_press_x, scene.mouse_press_y, x, y);
        }
        if (scene.isDrawingMode)
        {
            scene.finalizeDrawing();
        }
    }

    if (button == OF_MOUSE_BUTTON_RIGHT)
    {
        isRightMouseButtonDown = false;
    }
}

void MouseEventHandler::handleMouseDragged(int x, int y, int button)
{
    Application& app = Application::getInstance();
    Scene& scene = app.getScene();
    User_Camera_Movement& cameraMovement = app.getUserCameraMovement();
    ofCamera& camera = cameraMovement.camera;

    if (isMouseOverAnyGui(x, y)) return;

    scene.mouse_current_x = x;
    scene.mouse_current_y = y;

    if (scene.isDrawingMode && button == OF_MOUSE_BUTTON_LEFT)
    {
        scene.updateCurrentDrawing(x, y);
        return;
    }

    if (isRightMouseButtonDown && button == OF_MOUSE_BUTTON_RIGHT)
    {
        ofVec2f mouseDelta = ofVec2f(x, y) - scene.lastMouseScreen;
        cameraMovement.rotateCamera(-mouseDelta.x, -mouseDelta.y);
        scene.lastMouseScreen.set(x, y);
        return;
    }

    if (scene.is_mouse_button_pressed && button == OF_MOUSE_BUTTON_LEFT)
    {
        switch (scene.currentTransform)
        {
            case Scene::TransformMode::Translate:
            {
                ofVec3f delta = ofVec3f(x, y, 0) - scene.lastMouseScreen;
                ofVec3f right = camera.getXAxis();
                ofVec3f up = camera.getYAxis();
                float sensitivity = 0.01f;

                for (Object3D* object : scene.selectedObjects)
                {
                    object->transformPosition(right * delta.x * sensitivity);
                    object->transformPosition(up * -delta.y * sensitivity);
                }
                break;
            }
            case Scene::TransformMode::Resize:
            {
                float scaleFactor = scene.scaleFactor;
                float deltaScale = -(y - scene.lastMouseScreen.y) * scaleFactor;

                for (Object3D* object : scene.selectedObjects)
                {
                    ofVec3f newScale = object->getScale() + deltaScale;
                    newScale.x = std::max(1.0f, newScale.x);
                    newScale.y = std::max(1.0f, newScale.y);
                    newScale.z = std::max(1.0f, newScale.z);
                    object->setScale(newScale);
                }
                break;
            }
            case Scene::TransformMode::Rotate:
            {
                ofVec2f delta = ofVec2f(x, y) - scene.lastMouseScreen;
                float rotationSpeed = scene.rotationSpeed;

                for (Object3D* object : scene.selectedObjects)
                {
                    ofVec3f rotationDelta(delta.y * rotationSpeed, delta.x * rotationSpeed, 0);
                    object->transformRotation(rotationDelta);
                }
                break;
            }
            default:
                break;
        }
    }

    scene.lastMouseScreen.set(x, y);
}

void MouseEventHandler::handleMouseMoved(int x, int y)
{
    Application& app = Application::getInstance();
    Scene& scene = app.getScene();

    scene.mouse_current_x = x;
    scene.mouse_current_y = y;

    if (!scene.is_mouse_button_pressed &&
        scene.cursor.getState() != CursorState::MOVE &&
        scene.cursor.getState() != CursorState::RESIZE)
    {
        if (scene.isMouseOverObject(x, y))
        {
            scene.cursor.setState(CursorState::HOVER);
        }
        else
        {
            scene.cursor.setState(CursorState::DEFAULT);
        }
    }
}
