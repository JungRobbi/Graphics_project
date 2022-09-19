#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"

class LavaMove :
    public Component
{
public:
    float MoveSpeed = 0.01f;
    int count = 0;
    bool updown = true;

public:

    void start();
    void update();
};
