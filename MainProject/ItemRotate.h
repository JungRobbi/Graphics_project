#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"
class ItemRotate :
    public Component
{
public:
    float rotating_speed = 1.0f;

    void start() {}
    void update()
    {
        gameObject->GetComponent<Transform3D>()->yaw += rotating_speed;
    }
};

