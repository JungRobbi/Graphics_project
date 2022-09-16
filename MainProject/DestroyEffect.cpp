#include "DestroyEffect.h"

void DestroyEffect::update()
{
    if (!destroy)
        return;

    if (subObject) {
        --destroy_frame_time;
        if (destroy_frame_time <= 0)
            Scene::scene->PushDelete(gameObject);
        return;
    }

    for (int i{}; i < EFFECT_OBJECT_NUM; ++i) {
        auto box = Scene::scene->CreateEmpty();

        box->AddComponent<Transform3D>();
        box->AddComponent<Collide>();

        box->GetComponent<Collide>()->BoundBox = gameObject->GetComponent<Collide>()->BoundBox * 0.3f;
        box->AddComponent<Gravity>();
        box->AddComponent<DestroyEffect>();
        box->GetComponent<DestroyEffect>()->destroy = true;
        box->GetComponent<DestroyEffect>()->subObject = true;

        box->GetComponent<Transform3D>()->position = gameObject->GetComponent<Transform3D>()->position;
        box->GetComponent<Transform3D>()->scale.x = gameObject->GetComponent<Transform3D>()->scale.x * 0.3f;
        box->GetComponent<Transform3D>()->scale.y = gameObject->GetComponent<Transform3D>()->scale.y * 0.3f;
        box->GetComponent<Transform3D>()->scale.z = gameObject->GetComponent<Transform3D>()->scale.z * 0.3f;

        std::random_device rad;
        std::default_random_engine dre(rad());
        std::uniform_real_distribution<double> uid1{ -0.05, 0.05 };
        std::uniform_real_distribution<double> uid1abs{ 0.0, 0.05 };
        auto b = gameObject->GetComponent<Collide>()->BoundBox;
        std::uniform_real_distribution<double> uidx{ b.minX, b.maxX };
        std::uniform_real_distribution<double> uidy{ b.minY, b.maxY };
        std::uniform_real_distribution<double> uidz{ b.minZ, b.maxZ };
        float r_x = uidx(dre);
        float r_y = uidy(dre);
        float r_z = uidz(dre);
        
        box->GetComponent<Transform3D>()->position += glm::vec3(r_x, r_y, r_z);
        box->GetComponent<Transform3D>()->velocity = glm::vec3(uid1(dre), uid1abs(dre), uid1(dre));

        // render 부분
        box->modelLocation = gameObject->modelLocation;
        box->num_index = gameObject->num_index; // load() 첫 번째
        box->VAO = gameObject->VAO; // 사각형 메쉬
        box->texture = gameObject->texture; // 1번 텍스쳐
    }

    Scene::scene->PushDelete(gameObject);
}