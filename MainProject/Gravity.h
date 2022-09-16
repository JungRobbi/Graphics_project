#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"

class Gravity :
    public Component
{
public:
    glm::vec3 graviti_acceleration = glm::vec3(0.0f, -0.001f, 0.0f);

    float floor = 0.0f;

public:
    void start() {}
    void update()
    {
        auto object_tran = gameObject->GetComponent<Transform3D>();
        auto object_BoundBox = gameObject->GetComponent<Collide>();

        object_tran->velocity += graviti_acceleration;

        if (!object_BoundBox)
            return;

        if (object_tran->position.y + object_BoundBox->BoundBox.minY <= floor) {
            object_tran->position.y = floor - object_BoundBox->BoundBox.minY;
            object_tran->velocity.y = 0.0f;
        }
    }


};

