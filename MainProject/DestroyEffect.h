#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Scene.h"
class DestroyEffect :
    public Component
{

public:
    bool destroy = false;
    int destroy_frame_time = 1000;
    

    void start() {}
    void update()
    {
        if (!destroy)
            return;

        --destroy_frame_time;


        if (destroy_frame_time == 0)
            Scene::scene->PushDelete(gameObject);

    }
};

