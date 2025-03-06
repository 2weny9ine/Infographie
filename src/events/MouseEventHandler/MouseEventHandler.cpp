#include "MouseEventHandler.h"
#include "Application.h"

MouseEventHandler::MouseEventHandler()
    : isRightMouseButtonDown(false)
{}

void MouseEventHandler::handleMousePressed(int x, int y, int button)
{
    Application& app = Application::getInstance();
    Scene& scene = app.getScene();

    scene.is_mouse_button_pressed = true;
    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
    scene.mouse_press_x = x;
    scene.mouse_press_y = y;
    scene.cursor.setState(CursorState::PRESSED);

    if (scene.isDrawingMode)
    {
        scene.startDrawing(x, y);
    }

    if (button == 2) // Right mouse button pressed
    {
        isRightMouseButtonDown = true;
        // Store the initial mouse position
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

    if (scene.isMouseOverObject(x, y))
    {
        scene.cursor.setState(CursorState::HOVER);
    }
    else
    {
        scene.cursor.setState(CursorState::DEFAULT);
    }

    float distance = sqrt(pow(x - scene.mouse_press_x, 2) + pow(y - scene.mouse_press_y, 2));

    if (distance > 20.0f && scene.currentTransform == Scene::TransformMode::None)
    {
        scene.selectAllInBounds(scene.mouse_press_x, scene.mouse_press_y, x, y);
    }

    if (scene.isDrawingMode)
    {
        scene.finalizeDrawing();
    }

    if (button == 2) // Right mouse button released
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

    scene.mouse_current_x = x;
    scene.mouse_current_y = y;
    scene.cursor.setState(CursorState::DRAGGING);

    if (scene.isDrawingMode)
    {
        scene.updateCurrentDrawing(x, y);
        return;
    }

    // Check if the right mouse button is being held and dragged
    if (isRightMouseButtonDown)
    {
        // Calculate the delta movement
        ofVec2f mouseDelta = -(ofVec2f(x, y) - scene.lastMouseScreen);

        // Rotate the camera based on the mouse movement
        cameraMovement.rotateCamera(mouseDelta.x, mouseDelta.y);

        // Update the last mouse position
        scene.lastMouseScreen.set(x, y);       
    }

    Scene::TransformMode transformMode = scene.currentTransform;

    if (transformMode == Scene::TransformMode::Translate)
    {
        float nearClip = camera.getNearClip();
        float farClip = camera.getFarClip();
        ofVec3f worldNear = camera.screenToWorld({ x, y, nearClip });
        ofVec3f worldFar = camera.screenToWorld({ x, y, farClip });
        ofVec3f direction = (worldFar - worldNear).getNormalized();

        float t = (scene.planeZ - worldNear.z) / direction.z;
        ofVec3f mouseWorld = worldNear + direction * t;
        ofVec3f delta = mouseWorld - scene.lastMouseWorld;

        for (Object3D* object : scene.selectedObjects)
            object->transformPosition(delta);

        scene.lastMouseWorld = mouseWorld;
    }
    else if (transformMode == Scene::TransformMode::Resize)
    {
        ofVec2f mouseDelta = -(ofVec2f(x, y) - scene.lastMouseScreen);
        float scaleFactor = scene.scaleFactor;

        for (Object3D* object : scene.selectedObjects)
        {
            ofVec3f newScale = object->getScale();
            newScale += ofVec3f(mouseDelta.y * scaleFactor);
            newScale.x = std::max(1.0f, newScale.x);
            newScale.y = std::max(1.0f, newScale.y);
            newScale.z = std::max(1.0f, newScale.z);
            object->setScale(newScale);
        }

        scene.lastMouseScreen.set(x, y);
    }
    else if (transformMode == Scene::TransformMode::Rotate)
    {
        ofVec2f mouseDelta = ofVec2f(x, y) - scene.lastMouseScreen;
        float rotationSpeed = scene.rotationSpeed;
        float zRotationFactor = 0.5f * rotationSpeed;

        for (Object3D* object : scene.selectedObjects)
        {
            ofVec3f newRotation = object->getRotation();
            newRotation.x += mouseDelta.y * rotationSpeed;
            newRotation.y += mouseDelta.x * rotationSpeed;
            newRotation.z += (mouseDelta.x + mouseDelta.y) * zRotationFactor;
            object->setRotation(newRotation);
        }

        scene.lastMouseScreen.set(x, y);
    }
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
