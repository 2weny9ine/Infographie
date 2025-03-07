#include "User_Camera_Movement.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

User_Camera_Movement::User_Camera_Movement()
    : scene(nullptr),
    move_forward(false),
    move_backwards(false),
    move_left(false),
    move_right(false),
    move_upwards(false),
    move_downwards(false)
{}

void User_Camera_Movement::setup(Scene& s)
{
    scene = &s;
    camera.setPosition(0, 0, 800);
    camera.lookAt(glm::vec3(0, 0, 0));
}

void User_Camera_Movement::update(float time_elapsed)
{
    if (!scene) return;
    float moveSpeed = 5.0f; // Movement speed

    // Get the camera's orientation vectors
    glm::vec3 forward = glm::normalize(camera.getLookAtDir());
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::cross(right, forward);

    // Apply movement relative to the camera's orientation
    if (move_forward)
        camera.setPosition(camera.getPosition() + forward * moveSpeed);
    if (move_backwards)
        camera.setPosition(camera.getPosition() - forward * moveSpeed);
    if (move_left)
        camera.setPosition(camera.getPosition() - right * moveSpeed);
    if (move_right)
        camera.setPosition(camera.getPosition() + right * moveSpeed);
    if (move_upwards)
        camera.setPosition(camera.getPosition() + up * moveSpeed);
    if (move_downwards)
        camera.setPosition(camera.getPosition() - up * moveSpeed);
}

void User_Camera_Movement::rotateCamera(float deltaX, float deltaY)
{
    float rotationSpeed = 0.005f;

    glm::quat orientation = camera.getOrientationQuat();
    glm::quat yawQuat = glm::angleAxis(deltaX * rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchQuat = glm::angleAxis(deltaY * rotationSpeed, camera.getSideDir());

    orientation = glm::normalize(yawQuat * orientation);
    orientation = glm::normalize(pitchQuat * orientation);

    camera.setOrientation(orientation);

    std::cout << "Camera Rotated" << std::endl;
}
