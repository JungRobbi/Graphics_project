#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
class CannonShot :
    public Component
{
public:
    bool subObject = false;
    int destroy_frame_time = 100;
    int Shot_delay = 100;

    void start() {}
    void update();
};

