#include "User_Camera_Movement.h"

User_Camera_Movement::User_Camera_Movement()
    : scene(nullptr),
    move_forward(false),
    move_backwards(false),
    move_left(false),
    move_right(false),
    move_upwards(false),
    move_downwards(false)
{
}

void User_Camera_Movement::setup(Scene& s) {
    scene = &s;
    camera.setPosition(0, 0, -800);
    camera.lookAt(glm::vec3(0, 0, 0));
}

void User_Camera_Movement::update(float time_elapsed) {
    if (!scene) return;
    float moveSpeed = 5.0f; // Movement speed

    if (move_forward)
        camera.setPosition(camera.getPosition() + glm::vec3(0, 0, moveSpeed));
    if (move_backwards)
        camera.setPosition(camera.getPosition() - glm::vec3(0, 0, moveSpeed));
    if (move_left)
        camera.setPosition(camera.getPosition() + glm::vec3(moveSpeed, 0, 0));
    if (move_right)
        camera.setPosition(camera.getPosition() - glm::vec3(moveSpeed, 0, 0));
    if (move_upwards)
        camera.setPosition(camera.getPosition() + glm::vec3(0, moveSpeed, 0));
    if (move_downwards)
        camera.setPosition(camera.getPosition() - glm::vec3(0, moveSpeed, 0));
}
