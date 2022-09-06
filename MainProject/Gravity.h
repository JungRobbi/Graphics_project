#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"
class Gravity :
    public Component
{
public:
    glm::vec3 graviti_acceleration = glm::vec3(0.0f, -0.0001f, 0.0f);

public:
    void start() {}
    void update()
    {
        auto object_tran = gameObject->GetComponent<Transform3D>();
        auto object_BoundBox = gameObject->GetComponent<Collide>()->BoundBox;

        object_tran->velocity += graviti_acceleration;

        if (object_tran->position.y + object_BoundBox.minY <= 0.0f) {
            object_tran->position.y = 0.0f - object_BoundBox.minY;
            object_tran->velocity.y = 0.0f;
        }
    }


};

