#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"
class ItemRotate :
    public Component
{
public:
    float rotating_speedx = 1.0f;
    float rotating_speedy = 1.0f;
    float rotating_speedz = 1.0f;

    bool rotating_axis[3]{
        false, true, false
    };

    int turn = 50;

    void start() {}
    void update()
    {
        if (rotating_axis[0])
            gameObject->GetComponent<Transform3D>()->pitch += rotating_speedx;
        if (rotating_axis[1])
            gameObject->GetComponent<Transform3D>()->yaw += rotating_speedy;
        if (rotating_axis[2])
            gameObject->GetComponent<Transform3D>()->roll += rotating_speedz;

        --turn;
        if (turn > 0)
            gameObject->GetComponent<Transform3D>()->position.y += 0.004;
        else
            gameObject->GetComponent<Transform3D>()->position.y -= 0.004;

        if (turn == -49)
            turn = 50;
    }
};

