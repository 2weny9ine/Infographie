#pragma once
#include "ofMain.h"
#include "Scene.h"

class User_Camera_Movement {
public:
    Scene* scene;

    bool move_forward;
    bool move_backwards;
    bool move_left;
    bool move_right;
    bool move_upwards;
    bool move_downwards;

    User_Camera_Movement();

    void setup(Scene& s);

    void update(float time_elapsed);
};
