#include "CannonShot.h"
#include "Transform3D.h"
#include "Gravity.h"
#include "ItemRotate.h"
#include "DestroyEffect.h"

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution<int> uidi(50, 150);
std::uniform_int_distribution<int> uid2(14, 16);

extern BoundingBox BoundBox[10];

void CannonShot::start()
{
    Shot_delay = uidi(dre);
}

void CannonShot::update()
{
	if (subObject) {
		--destroy_frame_time;
        if (destroy_frame_time <= 0) {
            Scene::scene->PushDelete(gameObject);
        }
		return;
	}

    --Shot_delay;
    if (Shot_delay <= 0) {
        auto ball = Scene::scene->CreateEmpty();

        ball ->AddComponent<Transform3D>();
        ball->AddComponent<Collide>();
        ball->GetComponent<Collide>()->BoundBox = BoundBox[Cannon];
        ball->GetComponent<Collide>()->BoundBox.pos = ball->GetComponent<Transform3D>()->position;

        ball->AddComponent<Gravity>();
        ball->AddComponent<CannonShot>();

        ball->GetComponent<CannonShot>()->subObject = true;


        ball->GetComponent<Transform3D>()->position = gameObject->GetComponent<Transform3D>()->position + glm::vec3(0.0f, 0.6f, 0.8f);
        ball->GetComponent<Transform3D>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);

        
        ball->GetComponent<Transform3D>()->velocity = glm::vec3(0, 0.03f, 0.15f);

        // render 부분
        ball->modelLocation = gameObject->modelLocation;
        ball->num_index = Scene::scene->p_index_list[Ball]; // load() 첫 번째
        ball->VAO = Scene::scene->p_vao[Ball]; // 사각형 메쉬
        ball->texture = Scene::scene->p_tex[uid2(dre)]; // 

        Shot_delay = uidi(dre);
	}
}