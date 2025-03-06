#include "MouseDraggedEventHandler.h"
#include "Application.h"

MouseDraggedEventHandler::MouseDraggedEventHandler() {}

void MouseDraggedEventHandler::handleMouseDragged(int x, int y, int button)
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
