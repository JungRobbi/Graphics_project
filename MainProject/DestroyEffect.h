#pragma once
#include <random>
#include <cmath>

#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"
#include "Collide.h"
#include "Gravity.h"
#include "Scene.h"

const int EFFECT_OBJECT_NUM = 3*3*3;

class DestroyEffect :
    public Component
{

public:
    bool destroy = false;
    bool subObject = false;
    int destroy_frame_time = 100;
    

    void start() {}
    void update();
};

