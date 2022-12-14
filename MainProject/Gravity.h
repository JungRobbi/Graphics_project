#pragma once
#include <glm/glm.hpp>
#include "Component.h"
#include "GameObject.h"
#include "Transform3D.h"
#include <iostream>
class Gravity :
    public Component
{
public:
    glm::vec3 graviti_acceleration = glm::vec3(0.0f, -0.002f, 0.0f);

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


        if (gameObject == Scene::scene->p_player) {
            if (object_tran->position.y + object_BoundBox->sub_BoundBox.minY <= floor) {
                object_tran->position.y = floor - object_BoundBox->sub_BoundBox.minY;
                object_tran->velocity.y = 0.0f;
            }
        }
        else {
            if (object_tran->position.y + object_BoundBox->BoundBox.minY <= floor) {
                object_tran->position.y = floor - object_BoundBox->BoundBox.minY;
                object_tran->velocity.y = 0.0f;
            }
        }
    }


};

