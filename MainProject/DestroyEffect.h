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
    int destroy_frame_time = 100;
    

    void start() {}
    void update()
    {
        if (!destroy)
            return;


        //--destroy_frame_time;
        //if (destroy_frame_time >= 0)
        //  return;

        for (int i{}; i < EFFECT_OBJECT_NUM; ++i) {
            auto box = Scene::scene->CreateEmpty();

            box->AddComponent<Transform3D>();
            box->AddComponent<Collide>();
            box->GetComponent<Collide>()->BoundBox = BoundingBox{};

            box->AddComponent<Gravity>();

            box->GetComponent<Transform3D>()->position = gameObject->GetComponent<Transform3D>()->position;
            box->GetComponent<Transform3D>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);

            
            std::random_device rad;
            std::default_random_engine dre(rad());
            std::uniform_real_distribution<double> uid1{ -0.05, 0.05 };
            std::uniform_real_distribution<double> uid1abs{ 0.0, 0.1 };

            box->GetComponent<Transform3D>()->velocity = glm::vec3(uid1(dre), uid1abs(dre), uid1(dre));

            // render 부분
            box->modelLocation = gameObject->modelLocation;
            box->num_index = gameObject->num_index; // load() 첫 번째
            box->VAO = gameObject->VAO; // 사각형 메쉬
            box->texture = gameObject->texture; // 1번 텍스쳐
        }

        Scene::scene->PushDelete(gameObject);

    }
};

