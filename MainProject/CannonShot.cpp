#include "CannonShot.h"
#include "Transform3D.h"
#include "Gravity.h"
#include "ItemRotate.h"

void CannonShot::update()
{
	if (subObject) {
		--destroy_frame_time;
		if (destroy_frame_time <= 0)
			Scene::scene->PushDelete(gameObject);
		return;
	}

    --Shot_delay;
    if (Shot_delay <= 0) {
        auto ball = Scene::scene->CreateEmpty();

        ball ->AddComponent<Transform3D>();
    //  ball->AddComponent<Collide>();
        ball->AddComponent<Gravity>();
        ball->AddComponent<CannonShot>();
        ball->GetComponent<CannonShot>()->subObject = true;

        ball->GetComponent<Transform3D>()->position = gameObject->GetComponent<Transform3D>()->position;

        auto theta = gameObject->GetComponent<Transform3D>()->yaw;
        
        ball->GetComponent<Transform3D>()->velocity = glm::vec3(0.001f , 0.05f, 0.0f);

        // render 부분
        ball->modelLocation = gameObject->modelLocation;
        ball->num_index = Scene::scene->p_index_list[Ball]; // load() 첫 번째
        ball->VAO = Scene::scene->p_vao[Ball]; // 사각형 메쉬
        ball->texture = Scene::scene->p_tex[14]; // 

        Shot_delay = 100;
	}
}