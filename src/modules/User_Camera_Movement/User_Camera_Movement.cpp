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
}

void User_Camera_Movement::update(float time_elapsed) {
    if (!scene) return;

    if (move_forward)
        scene->offset_z += scene->delta_z * time_elapsed;
    if (move_backwards)
        scene->offset_z -= scene->delta_z * time_elapsed;
    if (move_left)
        scene->offset_x += scene->delta_x * time_elapsed;
    if (move_right)
        scene->offset_x -= scene->delta_x * time_elapsed;
    if (move_upwards)
        scene->offset_y += scene->delta_y * time_elapsed;
    if (move_downwards)
        scene->offset_y -= scene->delta_y * time_elapsed;
}
